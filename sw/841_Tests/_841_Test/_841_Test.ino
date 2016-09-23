// Two Address Slave Sender -- WireS library sample
//
// This code is derived from
//   http://www.arduino.cc/en/Tutorial/MasterReader
// Modified by Hisashi ITO (info at mewpro.cc) (c) 2015
// to demonstrate how to make slave with two addresses.
//
// Refer to the "Two Address Master Reader" example for use with this

// The original copyright follows:
// ------------------------------------------------------------
// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <WireS.h>
#include <Servo.h>

#define TOPSERVOPIN 2
#define BOTSERVOPIN 3
#define HIPSERVOPIN 5
#define CURRENTREADPIN A1

volatile uint8_t slaveAddr;
Servo topservo,botservo,hipservo;

uint8_t servovals[3];
int current_read;

long curtime = 0;
long servoupdatetime = 0;
long currentreadtime = 0;


int servoupdateinterval = 10;
int currentreadinterval = 10;

void setup()
{
  Wire.begin(4, (5 << 1 | 1));      // join i2c bus with addresses #2 and #3
  Wire.onAddrReceive(addressEvent); // register event
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);     // register event

  ADMUXB = 0b00000000;
  servovals[0] = 74;
  servovals[1] = 182;
  servovals[2] = 128;
  current_read = 0;

  pinMode(CURRENTREADPIN, INPUT);

  //ADC_SetVoltageReference(ADC_REFERENCE_VCC);

  //Set in stone
  topservo.attach(TOPSERVOPIN);
  botservo.attach(BOTSERVOPIN);
  hipservo.attach(HIPSERVOPIN);
}

void loop()
{
  curtime = millis();
  if(curtime - servoupdatetime > servoupdateinterval){
    servoupdatetime = curtime;
    topservo.writeMicroseconds(map(servovals[0],0,255,900,2100)); 
    botservo.writeMicroseconds(map(servovals[1],0,255,900,2100)); 
    hipservo.writeMicroseconds(map(servovals[2],0,255,900,2100)); 
  }
  
  if(curtime - currentreadtime > currentreadinterval){  
    currentreadtime = curtime;
    //updating analogpin value
    current_read = analogRead(CURRENTREADPIN);
  }
}

// function that executes whenever address is received from master
// this function is registered as an event, see setup()
boolean addressEvent(uint16_t address, uint8_t count)
{
  slaveAddr = (address >> 1);
  return true; // send ACK to master
}

void receiveEvent(size_t howMany){
  if(howMany > 3)
    howMany = 3;
  for(int i = 0; i < howMany; i++)
    servovals[i] = Wire.read();    // receive byte as an integer
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  switch (slaveAddr) {
  case 4:  
    for(int i = 0; i < 3; i++)
      Wire.write(servovals[i]);  
    break;
  case 5:
    Wire.write(highByte(current_read)); // respond with another message
    Wire.write(lowByte(current_read));
    break;
  }
}
