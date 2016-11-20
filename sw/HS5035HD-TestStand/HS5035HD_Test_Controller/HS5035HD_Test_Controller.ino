#include <Wire.h>
#include <Adafruit_INA219.h>
#include "Adafruit_TMP007.h"
#include "Servo.h"

Adafruit_INA219 ina219;
Adafruit_TMP007 tmp007;

Servo test_servo;
int servocounter = 0;

long curtime, infotime,servotime, talktime;

bool plotting = true;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float objt = 0;      
float diet = 0;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  ina219.begin(0x41);
  ina219.setCalibration_16V_400mA();
  delay(10);
  ///*
  if (! tmp007.begin(0x40)) {
    Serial.println("No TMP007 sensor found");
    while (1);
  }
  //*/

  test_servo.attach(10);
  infotime = 0;
  servotime = 0;
  talktime = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  curtime = millis();
  if(curtime - infotime > 100){
    infotime = curtime;
    //test_servo.writeMicroseconds(1500);
 
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    objt = tmp007.readObjTempC();
    diet = tmp007.readDieTempC();
  }
  if(curtime - talktime > 500){
    talktime = curtime;
     if(!plotting){
      Serial.println("CURRENT INFORMATION");
      Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
      Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
      Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
      Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
      
      Serial.println("TEMPERATURE INFORMATION");
      Serial.print("Object Temperature: "); Serial.print(objt); Serial.println("*C");
      Serial.print("Die Temperature: "); Serial.print(diet); Serial.println("*C");   
    }
    else{
      Serial.print(current_mA);
      Serial.print(",");
      Serial.print(objt);
      Serial.println();
    }
  }
  if(curtime - servotime > 10){
    servotime = curtime;
    //servocounter = (servocounter+1)%100;
    test_servo.writeMicroseconds(1500+600*sin(servocounter/100.0*2*PI));
  }
}
