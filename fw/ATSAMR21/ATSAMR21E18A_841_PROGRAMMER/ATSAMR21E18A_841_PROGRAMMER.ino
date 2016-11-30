#include <Arduino.h>
#include <Wire.h>

#include <SPI.h>

#define Serial SERIAL_PORT_USBVIRTUAL


uint8_t top_servo_address = 0x04;
uint8_t bot_servo_address = 0x02;

static uint8_t servovals[5];
static uint8_t servo_corrections[5] = {0,0,0,0,0};

bool ledstatus = true;
long curtime, servoupdatetime,updateledtime;

int i = 0;

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  servoupdatetime = 0;
  updateledtime = 0;  
  servovals[0] = 128;
  servovals[1] = 128;
  servovals[2] = 128;
  servovals[3] = 128;
  servovals[4] = 128;
}

void loop() {
  curtime = millis();
  // put your main code here, to run repeatedly:
  if(curtime - updateledtime > 1000){
    updateledtime = curtime;
    ledstatus = !ledstatus;
    digitalWrite(0,ledstatus);
  }
  if(curtime - servoupdatetime > 10){
    servoupdatetime = curtime;
   ///*
    Wire.beginTransmission(4);
      Wire.write(servovals[0]+servo_corrections[0]);
      Wire.write(servovals[1]+servo_corrections[1]);
      Wire.write(servovals[2]+servo_corrections[2]);
    Wire.endTransmission();
   // */
    /*
    Wire.beginTransmission(2);
      Wire.write(servovals[3]+servo_corrections[3]);
      Wire.write(servovals[4]+servo_corrections[4]);
      Wire.write(servovals[2]+servo_corrections[2]);
    Wire.endTransmission();
    */
    
  }
}
