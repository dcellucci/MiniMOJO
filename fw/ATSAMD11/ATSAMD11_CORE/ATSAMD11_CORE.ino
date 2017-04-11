#include <Wire.h>
#include <Servo.h>
#include "wiring_private.h" // pinPeripheral() function
#include <INA3221.h>
#include <max1720x.h>

//System wide defines
//Sets whether serial will get used (for debug)
#if defined(ARDUINO_CDC_ONLY) || defined(ARDUINO_CDC_HID_UART) || defined(ARDUINO_CDC_UART) ||  defined(ARDUINO_CDC_MIDI_HID_UART) || defined(ARDUINO_CDC_MSD_HID_UART) || defined(ARDUINO_CDC_MSD_MIDI_HID_UART)
  #define DEBUG_SERIAL
#endif

#define MOJORTC

#ifdef MOJORTC
  #include <RTC_CSL.h>
#endif

//Flag for if this board is closest to the main processor (top) (also controls hip)
bool topServoset = false;

//Servo Objects
Servo topServo,botServo,hipServo;
//Initial Values
uint8_t servoVals[3] = {128,128,180};

//Comms Buffer for communication with main port
uint8_t BUFFER[255];
uint8_t bufferSize;

uint8_t msgout[20];

//Initialize the I2C bus that communicates with the Coordinator
TwoWire subComms(&sercom0,14,15);

//Initialize a current sensor object
INA3221 curSensor(0x43);
//Initialize a battery sensor object
MAX1720x batSensor;
//Initialize an RTC Counter
#ifdef MOJORTC
  RTC_CSL rtc;
#endif
  unsigned long curcount;
unsigned long curTime;
unsigned long servoUpdateTime, sensorUpdateTime;

unsigned long servoUpdateInterval,sensorUpdateInterval;


//Toggle Servo Power Supply
int servoPowerPin = 16;
bool servoPower = false;

//sensor values
uint16_t vshunts[3];
uint16_t vbuses[3];
uint16_t soc;



void setup() {
  servoUpdateInterval = 10000; //microseconds
  sensorUpdateInterval = 10000; //microseconds
  // put your setup code here, to run once:
#ifdef DEBUG_SERIAL
    SerialUSB.begin(9600);    
    servoPower = true;
#endif

  if(topServoset)
    subComms.begin(4);
  else
    subComms.begin(5);

  subComms.onReceive(receiveEvent);
  subComms.onRequest(requestEvent);
  
  Wire.begin();
  
  topServo.attach(4);
  botServo.attach(5);
  hipServo.attach(6);

  pinPeripheral(14, PIO_SERCOM);
  pinPeripheral(15, PIO_SERCOM);
  
  pinMode(servoPowerPin,OUTPUT);

  //initialize sensors
  curSensor.init();
  batSensor.reset();
#ifdef MOJORTC
  rtc.begin();
#endif
  curcount = 0;
}

void loop() {
  curTime = micros();
  if(curTime - servoUpdateTime > servoUpdateInterval){
    servoUpdateTime = curTime;
    topServo.writeMicroseconds(map(servoVals[0],0,255,900,2100));
    botServo.writeMicroseconds(map(servoVals[1],0,255,900,2100));
    hipServo.writeMicroseconds(map(servoVals[2],0,255,900,2100));
    digitalWrite(servoPowerPin, servoPower);
  }
  if(curTime - sensorUpdateTime > sensorUpdateInterval){
    sensorUpdateTime = curTime;  
    //get timestamp
#ifdef MOJORTC
    curcount = rtc.getCount();
#endif
    updateSensors();
  }
}

void receiveEvent(int howMany){
  memset(&BUFFER[0], 0, sizeof(BUFFER));
  bufferSize=0;
  
#ifdef DEBUG_SERIAL
    SerialUSB.print("Receiving");
    SerialUSB.println(howMany);
#endif

  char c;
  while(0 < subComms.available()){ // loop through all but the last
    c = subComms.read(); // receive byte as a character
    BUFFER[bufferSize] = (uint8_t)c;
    bufferSize++;
    
#ifdef DEBUG_SERIAL
    SerialUSB.print(c);         // print the character
#endif
  }
#ifdef DEBUG_SERIAL
  SerialUSB.print("\n");       
#endif
  parseBuffer();
}

void parseBuffer(){
  if(bufferSize > 0){
    switch((char)BUFFER[0]){
      case 'w':
        servoVals[0] = BUFFER[1];
        servoVals[1] = BUFFER[2];
        servoVals[2] = BUFFER[3];
        if(BUFFER[4] == '+')
          servoPower = true;
        else if(BUFFER[4] == '-')
          servoPower = false;
        break;
    }
  }
}

void requestEvent(){
  memset(msgout,0,sizeof(msgout));
  
  msgout[0] = (curcount>>24) & 0xFF;
  msgout[1] = (curcount>>16) & 0xFF;
  msgout[2] = (curcount>>8) & 0xFF;
  msgout[3] = curcount & 0xFF;
  
  for(int i = 0; i < 3; i++){
    msgout[i*2+4] = (vshunts[i]>>8) & 0xFF;
    msgout[i*2+5] = (vshunts[i]) & 0xFF;
    msgout[i*2+10] = (vbuses[i]>>8) & 0xFF;
    msgout[i*2+11] = (vbuses[i]) & 0xFF;
  }
  
  msgout[16] = (soc>>8)&0xFF;
  msgout[17] = (soc & 0xFF);

  subComms.write(msgout,18);
}

void updateSensors(){
  for(int i = 0; i < 3; i++){
    vshunts[i] = curSensor.read_shunt_voltage(i+1);
    vbuses[i] = curSensor.read_bus_voltage(i+1);
  }    
  soc = batSensor.getSOC();
#ifdef DEBUG_SERIAL
  SerialUSB.print("Current Count: ");
  SerialUSB.print(curcount);
  SerialUSB.println("INA3221:");
  for(int i = 0; i < 3; i++){
    SerialUSB.print("\tChannel ");
    SerialUSB.print(i+1);
    SerialUSB.println(":");
    SerialUSB.print("\t\tV_Shunt: ");
    SerialUSB.println(vshunts[i]);
    SerialUSB.print("\t\tV_Bus: ");
    SerialUSB.println(vbuses[i]);
  }
  SerialUSB.print("MAX17201 SoC: ");
  SerialUSB.println(soc);
#endif
}

extern "C" {
  void SERCOM0_Handler(void);

  void SERCOM0_Handler(void) {
    subComms.onService();
  }
}


