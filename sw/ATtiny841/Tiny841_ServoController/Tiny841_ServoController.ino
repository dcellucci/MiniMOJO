

#include <WireS.h>
#include <Servo.h>

#define TOPSERVOPIN 2
#define BOTSERVOPIN 3
#define HIPSERVOPIN 5

volatile uint8_t slaveAddr;
Servo topservo,botservo,hipservo;

uint8_t servovals[3];

long curtime = 0;
long servoupdatetime = 0;


int servoupdateinterval = 10;

void setup()
{
  //Top Servo Controller
  Wire.begin(4, (5 << 1 | 1));      // join i2c bus with addresses #2 and #3
  //Bot servo controller
  //Wire.begin(2, (3 << 1 | 1));
  
  Wire.onAddrReceive(addressEvent); // register event
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);     // register event

  servovals[0] = 74;
  servovals[1] = 182;
  servovals[2] = 175;

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
  for(int i = 0; i < 3; i++)
    Wire.write(servovals[i]);  
}
