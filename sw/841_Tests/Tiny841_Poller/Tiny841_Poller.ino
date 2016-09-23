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

float runavg = 0;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  SerialUSB.begin(115200);  // start serial for output
  SerialUSB.println("Startin up...");
}

void loop() {
  curtime = millis();
  if(SerialUSB.available()>0){
    char inval = SerialUSB.read();
    char inval2;
    switch(inval){
      case 's':
        inval2 = SerialUSB.read();
        if(inval2 == 't')
          printServoVals(true);
        else
          printServoVals(false);
        break;
      case 'c':
        inval2 = SerialUSB.read();
        if(inval2 == 't')
          printCurrentVals(true);
        else
          printCurrentVals(false);
        break;
      case 'w':
        inval2 = SerialUSB.read();
        if(inval2 == 't')
          setServoVals(true);
        else
          setServoVals(false);
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
    SerialUSB.print(", ");
    SerialUSB.print(botservoval);
    SerialUSB.print(", ");    

    printCurrentVals(true);
  }
  
}

void printServoVals(boolean top){
  int address = 2;
  if(top)
    address = 4;
  Wire.requestFrom(address, 3);    // request 6 bytes from slave device #8

  SerialUSB.print("Current Servo Values: ");
  while (Wire.available()) { // slave may send less than requested
    int c = Wire.read(); // receive a byte as character
    SerialUSB.print(c);         // print the character
    SerialUSB.print(" ");
  }
  SerialUSB.println(" ");
}

void setServoVals(boolean top){
  topservoval = SerialUSB.parseInt();
  botservoval = SerialUSB.parseInt();
  uint8_t hipservoval = SerialUSB.parseInt();
  int address = 2;
  if(top)
    address = 4;
  Wire.beginTransmission(address);
    Wire.write(topservoval);
    Wire.write(botservoval);
    Wire.write(hipservoval);
  Wire.endTransmission();

  printServoVals(top);
}

void setServoVals(int top, int bot, int hip){
  Wire.beginTransmission(4);
    Wire.write(top);
    Wire.write(bot);
    Wire.write(hip);
  Wire.endTransmission();
}

void printCurrentVals(boolean top){
  int address = 3;
  if(top)
    address = 5;
  Wire.requestFrom(address, 2);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    byte c1 = Wire.read(); // receive a byte as character
    byte c2 = Wire.read();
    uint16_t curval = (c1<<8 | c2);
    //SerialUSB.println(c1);
    //SerialUSB.println(c2);
    runavg = runavg + (map(curval,500,550,0,128)-runavg)/10.0;
    SerialUSB.println(runavg);         
  }
}

