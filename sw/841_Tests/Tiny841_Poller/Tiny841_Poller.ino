// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.
#include <Wire.h>


long curtime = 0;
long oscupdatetime = 0;

int osccounter= 0;
int oscinc = 5;

int oscupdateint = 25;

boolean oscillate = false;

uint8_t topservoval = 0;
uint8_t botservoval = 0;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  SerialUSB.begin(115200);  // start serial for output
  SerialUSB.println("Startin up...");
}

void loop() {
  curtime = millis();
  if(SerialUSB.available()>0){
    char inval = SerialUSB.read();
    switch(inval){
      case 's':
        printServoVals();
        break;
      case 'c':
        printCurrentVals();
        break;
      case 'w':
        setServoVals();
        break;
      case 'o':
        oscillate = !oscillate;
    }
  }

  if(curtime - oscupdatetime > oscupdateint && oscillate){
    oscupdatetime = curtime;
    osccounter = osccounter + oscinc;
    if(osccounter > 98)
      oscinc = -5;
    if(osccounter < 2)
      oscinc = 5;
    topservoval = int(74.0 + map(osccounter,1.0,100.0,0.0,40.0));
    botservoval = int(192.0- map(osccounter,1.0,100.0,0.0,40.0));
    setServoVals(topservoval,botservoval,0);
    
    SerialUSB.print(topservoval);
    SerialUSB.print(" ");
    SerialUSB.println(botservoval);
  }
  
}

void printServoVals(){
  Wire.requestFrom(2, 3);    // request 6 bytes from slave device #8

  SerialUSB.print("Current Servo Values: ");
  while (Wire.available()) { // slave may send less than requested
    int c = Wire.read(); // receive a byte as character
    SerialUSB.print(c);         // print the character
    SerialUSB.print(" ");
  }
  SerialUSB.println(" ");
}

void setServoVals(){
  topservoval = SerialUSB.parseInt();
  botservoval = SerialUSB.parseInt();
  uint8_t hipservoval = SerialUSB.parseInt();

  Wire.beginTransmission(2);
    Wire.write(topservoval);
    Wire.write(botservoval);
    Wire.write(hipservoval);
  Wire.endTransmission();

  printServoVals();
}

void setServoVals(int top, int bot, int hip){
  Wire.beginTransmission(2);
    Wire.write(top);
    Wire.write(bot);
    Wire.write(hip);
  Wire.endTransmission();
}

void printCurrentVals(){
  Wire.requestFrom(3, 2);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    byte c1 = Wire.read(); // receive a byte as character
    byte c2 = Wire.read();
    long curval = (c2<<8 | c1);
    SerialUSB.println(curval);         
  }
}

