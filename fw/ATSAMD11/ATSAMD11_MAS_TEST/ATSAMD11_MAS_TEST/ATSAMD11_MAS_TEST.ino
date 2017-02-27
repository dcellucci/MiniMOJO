#include <Wire.h>
#include "wiring_private.h" // pinPeripheral() function

#define MAX17201_ADDRESS 0x6C

uint16_t val;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  SerialUSB.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  wireReadRegister(0x0A,&val);
  //SerialUSB.println(val,BIN);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN,LOW);
  delay(1000);
}

void wireReadRegister(uint8_t reg, uint16_t *value)
{
  Wire.beginTransmission(MAX17201_ADDRESS);
    Wire.write(reg);                       // Register
  Wire.endTransmission();
  
  //delayMicroseconds(30); // Max 12-bit conversion time is 586us per sample

  Wire.requestFrom(MAX17201_ADDRESS+1, (uint8_t)4);  
  // Shift values to create properly formed integer
  SerialUSB.println(((Wire.read() << 8) | Wire.read()));
  SerialUSB.println(((Wire.read() << 8) | Wire.read()));
  //*value = ((Wire.read() << 8) | Wire.read());

}
