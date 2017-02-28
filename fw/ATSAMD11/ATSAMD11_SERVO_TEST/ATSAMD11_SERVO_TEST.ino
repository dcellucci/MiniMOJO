#include <Servo.h>

Servo topservo, botservo, hipservo;
int pos = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  //SerialUSB.begin(9600);
  topservo.attach(4);
  botservo.attach(5);
  hipservo.attach(6);
  
  topservo.writeMicroseconds(1500);
  botservo.writeMicroseconds(1500);
  //hipservo.writeMicroseconds(1500);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  topservo.writeMicroseconds(1500);
  botservo.writeMicroseconds(1500);
  hipservo.writeMicroseconds(2365);
  //SerialUSB.println("Hello");
  delay(1000);
}
