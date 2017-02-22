// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include "wiring_private.h" // pinPeripheral() function

TwoWire masComms(&sercom1,22,23);

void setup()
{
  masComms.begin(); // join i2c bus (address optional for master)
  pinPeripheral(22, PIO_SERCOM);
  pinPeripheral(23, PIO_SERCOM);

  pinMode(LED_BUILTIN,OUTPUT);
}

byte x = 0;

void loop()
{
  masComms.beginTransmission(0x02); // transmit to device #4
  masComms.write("x is ");        // sends five bytes
  masComms.write(x);              // sends one byte  
  masComms.endTransmission();    // stop transmitting
  x++;
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(1000);
  
  masComms.beginTransmission(0x02); // transmit to device #4
  masComms.write("x is ");        // sends five bytes
  masComms.write(x);              // sends one byte  
  masComms.endTransmission();    // stop transmitting
  x++;
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); 
}
