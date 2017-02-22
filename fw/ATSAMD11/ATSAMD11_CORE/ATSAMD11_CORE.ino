#include <Wire.h>
#include <Servo.h>
#include "wiring_private.h" // pinPeripheral() function

Servo topServo,botServo,hipServo;

uint8_t servoVals[3] = {128,128,128};

//Comms Buffer for communication with main port
uint8_t BUFFER[255];
uint8_t bufferSize;

TwoWire subComms(&sercom0,14,15);
TwoWire masComms(&sercom1,22,23);

long curTime;
long servoUpdateTime, sensorUpdateTime;

int servoUpdateInterval,sensorUpdateInterval;

bool topServoset = true; //Checks if we're the servoset closest to the main processor (top)

//Toggle Servo Power Supply
int servoPowerPin = 16;
bool servoPower = false;


#if defined(ARDUINO_CDC_ONLY) || defined(ARDUINO_CDC_HID_UART) || defined(ARDUINO_CDC_UART) ||  defined(ARDUINO_CDC_MIDI_HID_UART) || defined(ARDUINO_CDC_MSD_HID_UART) || defined(ARDUINO_CDC_MSD_MIDI_HID_UART)
  #define DEBUG_SERIAL
#endif

void setup() {
  servoUpdateInterval = 100;
  sensorUpdateInterval = 100;
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
  
  masComms.begin();
  
  topServo.attach(4);
  botServo.attach(5);
  hipServo.attach(6);

  pinPeripheral(14, PIO_SERCOM);
  pinPeripheral(15, PIO_SERCOM);
  pinPeripheral(22, PIO_SERCOM);
  pinPeripheral(23, PIO_SERCOM);

  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(servoPowerPin,OUTPUT);
}

void loop() {
  curTime = millis();
  if(curTime - servoUpdateTime > servoUpdateInterval){
    servoUpdateTime = curTime;
    topServo.writeMicroseconds(servoVals[0]);
    botServo.writeMicroseconds(servoVals[1]);
    hipServo.writeMicroseconds(servoVals[2]);
    digitalWrite(servoPowerPin, servoPower);
  }
  if(curTime - sensorUpdateTime > sensorUpdateInterval){
    updateSensors();
  }
}

void receiveEvent(int howMany){
  memset(&BUFFER[0], 0, sizeof(BUFFER));
#if defined(DEBUG_SERIAL)
    SerialUSB.print("Receiving");
    SerialUSB.println(howMany);
#endif
  char c;
  while(0 < subComms.available()){ // loop through all but the last
    c = subComms.read(); // receive byte as a character
    BUFFER[bufferSize] = (uint8_t)c;
    bufferSize++;
#if defined(DEBUG_SERIAL)
    SerialUSB.print(c);         // print the character
#endif
  }
#if defined(DEBUG_SERIAL)
  SerialUSB.print("\n");         // print the integer
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
        break;
      case 'x': 
        switch((char)BUFFER[1]){
          case 'p':
            if(BUFFER[2] == 0)
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
  
}

extern "C" {
  void SERCOM0_Handler(void);

  void SERCOM0_Handler(void) {
    subComms.onService();
  }
}


