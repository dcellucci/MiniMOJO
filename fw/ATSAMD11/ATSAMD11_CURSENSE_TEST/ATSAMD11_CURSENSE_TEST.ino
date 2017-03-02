#include <Wire.h>
#include <INA3221.h>
//#include <max1720x.h>


INA3221 curSensor(0x43);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  SerialUSB.begin(115700);
  pinMode(LED_BUILTIN, OUTPUT);

  curSensor.init();
  
  
  /*
  Wire.beginTransmission(INA3221_Address);
    Wire.write(0x00);
    Wire.write(0x80);
    Wire.write(0x00);
  Wire.endTransmission();
  delay(50);
  Wire.beginTransmission(INA3221_Address);
    Wire.write(0x00);
    Wire.write(0x71);
    Wire.write(0x27);
  Wire.endTransmission();
  */
  delay(50);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  // put your main code here, to run repeatedly:
  //delay(1000);
  SerialUSB.print("Channel 1 V Bus: ");
  SerialUSB.print(curSensor.read_bus_voltage(1));
  SerialUSB.print(" Channel 1 V Shunt: ");
  SerialUSB.println(curSensor.read_shunt_voltage(1),HEX);
  SerialUSB.print("Channel 2 V Bus: ");
  SerialUSB.print(curSensor.read_bus_voltage(2));
  SerialUSB.print(" Channel 2 V Shunt: ");
  SerialUSB.println(curSensor.read_shunt_voltage(2),HEX);
  SerialUSB.print("Channel 3 V Bus: ");
  SerialUSB.print(curSensor.read_bus_voltage(3));
  SerialUSB.print(" Channel 3 V Shunt: ");
  SerialUSB.println(curSensor.read_shunt_voltage(3),HEX);
  //digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
