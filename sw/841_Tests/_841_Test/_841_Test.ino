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
#define CURRENTREADPIN 9

volatile uint8_t slaveAddr;
Servo topservo,botservo,hipservo;

uint8_t topservoval, botservoval, hipservoval;
uint16_t current_read;

void setup()
{
  Wire.begin(2, (3 << 1 | 1));      // join i2c bus with addresses #2 and #3
  Wire.onAddrReceive(addressEvent); // register event
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);     // register event

  topservoval = 74;
  botservoval = 182;
  hipservoval = 128;

  //Set in stone
  topservo.attach(TOPSERVOPIN);
  botservo.attach(BOTSERVOPIN);
  hipservo.attach(HIPSERVOPIN);
}

void loop()
{
  delay(100);
  //updating Servo Values
  topservo.writeMicroseconds(map(topservoval,0,255,900,2100)); 
  botservo.writeMicroseconds(map(botservoval,0,255,900,2100)); 
  hipservo.writeMicroseconds(map(hipservoval,0,255,900,2100)); 
  //updating analogpin value
  current_read = analogRead(CURRENTREADPIN);
}

// function that executes whenever address is received from master
// this function is registered as an event, see setup()
boolean addressEvent(uint16_t address, uint8_t count)
{
  slaveAddr = (address >> 1);
  return true; // send ACK to master
}

void receiveEvent(size_t howMany){
  topservoval = Wire.read();    // receive byte as an integer
  botservoval = Wire.read();    // receive byte as an integer
  hipservoval = Wire.read();    // receive byte as an integer
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  switch (slaveAddr) {
  case 2:
    Wire.write(topservoval); // respond with message of 6 bytes
    Wire.write(botservoval);
    Wire.write(hipservoval);// as expected by master
    break;
  case 3:
    Wire.write(current_read); // respond with another message
    break;
  }
}
