#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#define Serial SERIAL_PORT_USBVIRTUAL

//CSL extension for the INA219 sensor
#include <MOJO_INA219.h>


uint8_t top_cur_address = 0x40;
Adafruit_INA219 top_cur_sen;

float top_cur_mA = 0;
float correction_factor = 1.375;

bool led_val = false;
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  
  top_cur_sen.begin(top_cur_address);
  top_cur_sen.setCalibration_MOJO();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  top_cur_mA = top_cur_sen.getCurrent_mA()*correction_factor;

  Serial.println(top_cur_mA);
  led_val = !led_val;
  digitalWrite(0,led_val);

  Wire.beginTransmission(4);
      Wire.write(128);
      Wire.write(128);
      Wire.write(128);
  Wire.endTransmission();

    Wire.requestFrom(4, 3);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
    Serial.print(c,DEC);         // print the character
    Serial.print(" ");
  }
  Serial.print("\n");
  delay(1000);

}
