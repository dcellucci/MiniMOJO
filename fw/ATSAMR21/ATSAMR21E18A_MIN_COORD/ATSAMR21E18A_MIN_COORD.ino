#include <Arduino.h>
#include <Wire.h>

#include <SPI.h>

#define Serial SERIAL_PORT_USBVIRTUAL

#include "lwm.h"
#include "lwm/sys/sys.h"
#include "lwm/nwk/nwk.h"

extern "C" {
  void println(char *x) { Serial.println(x); Serial.flush(); }
}

 
// LWM mesh methods 
//Send the message
static void sendMessage();
//Checks for confirmation (freezes otherwise)
static void appDataConf(NWK_DataReq_t *req);
//
//Wireless Comms Functions 
//
//Sends Current Status, single packet
static bool sendStatus(NWK_DataInd_t *ind);
//Updates the motor positions
static bool updateMotors(NWK_DataInd_t *ind);
//Alters Specific Settings
static bool changeSetting(NWK_DataInd_t *ind);

//Are we using serial comms?
boolean usingSerial = true;

//1 is MOJO control
//2 is USB bridge
int meshAddress = 1;

//Static data packet types (prevents memory leak)
//LWM data request struct
static NWK_DataReq_t nwkDataReq;

//Payload array (10 byte limit right now)
static uint8_t payload[30];
static uint8_t indata[5];
                                 
static uint8_t servovals[5];

uint8_t* rec_message;


boolean ledstatus = true; //for debug
boolean moving = false;
boolean hipmoving = false;
boolean currentping = false;

boolean top_servopower = false;
boolean bot_servopower = false;

/* TIMING VARIABLES
 *  
 */
unsigned long curtime = 0;
unsigned long ledupdatetime = 0;
unsigned long servoupdatetime = 0;
unsigned long sensorreadtime = 0;
unsigned long sensorsendtime = 0;
unsigned long nextframetime = 0;

unsigned long servoUpdateInterval = 10000; //microseconds
unsigned long sensorUpdateInterval = 10000; //microseconds


/*I2C Settings
 * 
 */
uint8_t top_servo_ctrlr = 0x04;
uint8_t bot_servo_ctrlr = 0x05;

uint16_t vshunts[6], vbuses[6], socs[2];



void setup() {
  if(usingSerial)
    Serial.begin(115200);

  //All this was from LWM example. Thanks Atmel.
  SPI.usingInterrupt(digitalPinToInterrupt(PIN_SPI_IRQ));
    
  SPI.beginTransaction(
      SPISettings(
          MODULE_AT86RF233_CLOCK, 
          MSBFIRST, 
          SPI_MODE0
      )
  );

  attachInterrupt(digitalPinToInterrupt(PIN_SPI_IRQ), HAL_IrqHandlerSPI, RISING);
  /*  wait for SPI to be ready  */
  delay(10);
  
  SYS_Init();
  NWK_SetAddr(meshAddress);
  NWK_SetPanId(0x01);
  PHY_SetChannel(0x1a);
  PHY_SetRxState(true);
  NWK_OpenEndpoint(1, sendStatus);
  NWK_OpenEndpoint(2, updateState);
  NWK_OpenEndpoint(3, changeSetting);
  pinMode(0,OUTPUT);

  Wire.begin();

  //Initialize Servo Values
  servovals[0] = 72;
  servovals[1] = 184;
  servovals[2] = 175;
  servovals[3] = 72;
  servovals[4] = 184;

}

void loop() {
  //Wireless comms communication
  SYS_TaskHandler();
  curtime = micros();  

  if(Serial.available() > 0)
    parseSerialMessage();
  
  
  if(curtime - ledupdatetime > 1000000){
    ledstatus = !ledstatus;
    ledupdatetime = curtime;
    digitalWrite(0, ledstatus);
  }
  
  
  if(curtime - servoupdatetime > servoUpdateInterval){
    servoupdatetime = curtime;
    updateControllerState();
  }

  if(curtime - sensorreadtime > sensorUpdateInterval){
    sensorreadtime = curtime;
    readSensorValues();
  }
}

void parseSerialMessage(){
  int curcount = 0;
  char comm[2];
  Serial.readBytesUntil(' ', comm, 2);
  while(Serial.available()){ 
    indata[curcount] = (uint8_t)Serial.parseInt();
    curcount++;
  }
  parseCommand(comm);
}

void parseCommand(char* comm){
  
  switch(comm[0]){
    case '!':
      switch(comm[1]){
        case 's':
          if(usingSerial){
            printSensorValues();
          }
          break;
      }
      break;
    case 's':
      break;
    case 'c':
      break;
    case 'w':
      break;
  } 
}

void printSensorValues(){
  SerialUSB.println("INA3221 A:");
  for(int i = 0; i < 3; i++){
    SerialUSB.print("\tChannel ");
    SerialUSB.print(i+1);
    SerialUSB.println(":");
    SerialUSB.print("\t\tV_Shunt: ");
    SerialUSB.println(vshunts[i]);
    SerialUSB.print("\t\tV_Bus: ");
    SerialUSB.println(vbuses[i]);
  }
  
  SerialUSB.println("INA3221 B:");
  for(int i = 0; i < 3; i++){
    SerialUSB.print("\tChannel ");
    SerialUSB.print(i+1);
    SerialUSB.println(":");
    SerialUSB.print("\t\tV_Shunt: ");
    SerialUSB.println(vshunts[i+3]);
    SerialUSB.print("\t\tV_Bus: ");
    SerialUSB.println(vbuses[i+3]);
  }
  SerialUSB.print("MAX17201 SoC A: ");
  SerialUSB.println(socs[0]);
  SerialUSB.print("MAX17201 SoC B: ");
  SerialUSB.println(socs[1]);
}

void readSensorValues(){
  sensorreadtime = curtime;
  Wire.requestFrom(top_servo_ctrlr,14);
  //if(Wire.available()){
    for(int i = 0; i < 3; i++){
      vshunts[i] = ((Wire.read() << 8) | Wire.read());
    }
    for(int i = 0; i < 3; i++){
      vbuses[i] = ((Wire.read() << 8) | Wire.read());
    }
    socs[0] = ((Wire.read() << 8) | Wire.read());
  //}
  Wire.requestFrom(bot_servo_ctrlr,14);
  //if(Wire.available()){
    for(int i = 0; i < 3; i++){
      vshunts[i+3] = ((Wire.read() << 8) | Wire.read());
    }
    for(int i = 0; i < 3; i++){
      vbuses[i+3] = ((Wire.read() << 8) | Wire.read());
    }
    socs[1] = ((Wire.read() << 8) | Wire.read());
  //}
}

void sendCurrentValues(){
  /*
  if(usingSerial){

    Serial.print(top_cur_mA);
    Serial.print(",");
    Serial.println(bot_cur_mA);
  }
  else{
    payload[0]= 'c';
    byte *top = (byte *)&top_cur_mA;
    payload[1] = top[0];
    payload[2] = top[1];
    payload[3] = top[2];
    payload[4] = top[3];
    byte *bot = (byte *)&bot_cur_mA;
    payload[5] = bot[0];
    payload[6] = bot[1];
    payload[7] = bot[2];
    payload[8] = bot[3];
  
    sendMessage();
    
  }*/
}




static void sendMessage(void){
  nwkDataReq.dstAddr = 2;
  nwkDataReq.dstEndpoint = 1;
  nwkDataReq.srcEndpoint = 1;
  nwkDataReq.options = 0;
  nwkDataReq.data = (uint8_t *)payload;
  nwkDataReq.size = sizeof(payload);
  nwkDataReq.confirm = appDataConf;
  NWK_DataReq(&nwkDataReq);
}

/*
 * Was having trouble with transmission before I
 * added this confirmation method.
 * It would send a single packet over the network and 
 * then freeze... this method fixed it.
 */
static void appDataConf(NWK_DataReq_t *req){
  if (NWK_SUCCESS_STATUS == req->status)
    Serial.println("Sent successfully");
  else
    Serial.println("Packet failed to send");
}

static bool sendStatus(NWK_DataInd_t *ind) {
  
  sendStatusMessage();
  //ledstatus = true;  
  return true;
}

void sendStatusMessage(){
  
  memset(payload,0,sizeof(payload));
  payload[5] = (top_servopower << 2) | (bot_servopower << 1); 
  for(int i = 0; i < 5; i++){ //The third channel of the bottom should be useless
    payload[i] = servovals[i];
    payload[i*2+6] = (vshunts[i]>>8) & 0xFF;
    payload[i*2+7] = (vshunts[i]) & 0xFF;
    payload[i*2+16] = (vbuses[i]>>8) & 0xFF;
    payload[i*2+17] = (vbuses[i]) & 0xFF;
  }
  payload[26] = (socs[0]>>8)&0xFF;
  payload[27] = (socs[0] & 0xFF);
  payload[28] = (socs[1]>>8)&0xFF;
  payload[29] = (socs[1] & 0xFF);

  sendMessage();
}

static bool updateState(NWK_DataInd_t *ind) {
  rec_message = (uint8_t*)(ind->data);

  if(usingSerial){
    Serial.print("Received message - ");
    Serial.print("lqi: ");
    Serial.print(ind->lqi, DEC);
  
    Serial.print("  ");
  
    Serial.print("rssi: ");
    Serial.print(ind->rssi, DEC);
    Serial.print("  ");

    Serial.println((char*)rec_message);
  }

  for(int i = 0; i < 5; i++)
    if(rec_message[i] != 0)
      servovals[i] = rec_message[i];
  top_servopower = rec_message[5]&0x04;
  bot_servopower = rec_message[5]&0x02;

  sendStatusMessage();
  
  return true;
}

static bool changeSetting(NWK_DataInd_t *ind){
  rec_message = (uint8_t*)(ind->data);
  parseCommand((char*)rec_message);
  return true;
}


void updateControllerState(){
    Wire.beginTransmission(top_servo_ctrlr);
      Wire.write('w');
      Wire.write(servovals[0]);
      Wire.write(servovals[1]);
      Wire.write(servovals[4]);
      if(top_servopower)
        Wire.write('+');
      else
        Wire.write('-');
    Wire.endTransmission();
    
    Wire.beginTransmission(bot_servo_ctrlr);
      Wire.write('w');
      Wire.write(servovals[2]);
      Wire.write(servovals[3]);
      Wire.write(servovals[4]);
      if(bot_servopower)
        Wire.write('+');
      else
        Wire.write('-');
    Wire.endTransmission();
}

