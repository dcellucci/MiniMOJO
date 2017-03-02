#include <Wire.h>
#include <Servo.h>
#include "wiring_private.h" // pinPeripheral() function
#include <INA3221.h>
#include <max1720x.h>

//Flag for if this board is closest to the main processor (top) (also controls hip)
bool topServoset = true;

//Servo Objects
Servo topServo,botServo,hipServo;
//Initial Values
uint8_t servoVals[3] = {128,128,180};

//Comms Buffer for communication with main port
uint8_t BUFFER[255];
uint8_t bufferSize;

//Initialize the I2C bus that communicates with the Coordinator
TwoWire subComms(&sercom0,14,15);

//Initialize a current sensor object
INA3221 curSensor(0x43);
//Initialize a battery sensor object
MAX1720x batSensor;

unsigned long curTime;
unsigned long servoUpdateTime, sensorUpdateTime;

unsigned long servoUpdateInterval,sensorUpdateInterval;


//Toggle Servo Power Supply
int servoPowerPin = 16;
bool servoPower = true;

//Sets whether serial will get used (for debug)
#if defined(ARDUINO_CDC_ONLY) || defined(ARDUINO_CDC_HID_UART) || defined(ARDUINO_CDC_UART) ||  defined(ARDUINO_CDC_MIDI_HID_UART) || defined(ARDUINO_CDC_MSD_HID_UART) || defined(ARDUINO_CDC_MSD_MIDI_HID_UART)
  #define DEBUG_SERIAL
#endif

void setup() {
  servoUpdateInterval = 10000; //microseconds
  sensorUpdateInterval = 10000; //microseconds
  // put your setup code here, to run once:
#if defined(DEBUG_SERIAL)
    SerialUSB.begin(9600);
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
  pinPeripheral(22, PIO_SERCOM);
  pinPeripheral(23, PIO_SERCOM);

  pinMode(servoPowerPin,OUTPUT);

  curSensor.init();
  batSensor.reset();
  //curSensor.enableDefaultSettings();
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
  if(bufferSize > 0)
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
      case '!': 
        switch((char)BUFFER[1]){
          case 'p':
            if((char)BUFFER[2] == '0')
              servoPower = false;
            else
              servoPower = true;
            break;
        }
        break;
    }
}

void requestEvent(){
  subComms.write("status");
}

void updateSensors(){
#ifdef DEBUG_SERIAL
  SerialUSB.print("INA3221 V_Shunt Channel 2: ");
  SerialUSB.println(curSensor.read_shunt_voltage(2));
  SerialUSB.print("MAX17201 SoC: ");
  SerialUSB.println(batSensor.getSOC());
#endif
}

extern "C" {
  void SERCOM0_Handler(void);

  void SERCOM0_Handler(void) {
    subComms.onService();
  }
}


