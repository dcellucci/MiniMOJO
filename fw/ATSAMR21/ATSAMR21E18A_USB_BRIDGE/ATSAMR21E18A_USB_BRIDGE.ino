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
static void sendMessage(int dstEndpointVal);
//Checks for confirmation (freezes otherwise)
static void appDataConf(NWK_DataReq_t *req);
//Receives a message
static bool receiveMessage(NWK_DataInd_t *ind);

//1 is MOJO control
//2 is USB bridge
int meshAddress = 2; 

//The received message
uint8_t *rec_message;

//Static data packet types (prevents memory leak)
//LWM data request struct
static NWK_DataReq_t nwkDataReq;
//Payload array (10 byte limit right now)
static uint8_t payload[9];
static uint8_t indata[5];

static uint8_t servovals[5] = {74,182,185,174,182};

//Command input (from Serial)
String command = "";

//Timing variables
long curtime = 0;


//for debug
boolean ledstatus = true; 
boolean debug = false;
long ledtoggletime = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("ATSAMR21E18A USB Bridge");

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
  
  //Associates a method with an endpoint value. 
  //Any value between 1 and 16 (0 is reserved)
  //Up to 16 different messages based on the value
  //of NWK.dstEndpoint.
  NWK_OpenEndpoint(1, receiveMessage);
}

void loop() {
  //This method should run as often as possible.
  SYS_TaskHandler();
  
  //Timing variable
  curtime = millis();

  //Reads a command from Serial if available
  if(Serial.available() > 0){
    parseCommand();
  }

  //Toggle LED stuff. More debug.
  if(curtime - ledtoggletime > 50){
    ledtoggletime = curtime;
    //ledstatus = !ledstatus;
    digitalWrite(0, ledstatus);
  }
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

void parseCommand(){
  memset(indata,0,sizeof(indata));
  memset(payload,0,sizeof(payload));
  int curcount = 0;
  char comm[2];
  Serial.readBytesUntil(' ', comm, 2);
  while(Serial.available()){ 
    indata[curcount] = (uint8_t)Serial.parseInt();
    curcount++;
  }
  switch(comm[0]){
    case '!':
      payload[0] = comm[0];
      payload[1] = comm[1];
      Serial.println((char*)payload);
      sendMessage(3);
      break;
    case 's':
      payload[0]='s';
      sendMessage(1);
      break;
    case 'l':
      ledstatus = !ledstatus;
    case 'w':
      switch(comm[1]){
        case 't':
          if(indata[0] > 0)
            servovals[0] = indata[0];
          if(indata[1] > 0)
            servovals[1] = indata[1];
          writeServoVals();
          break;
        case 'h':        
          if(indata[0] > 0)      
            servovals[2] = indata[0];
          writeServoVals();
          break;
        case 'b': 
          if(indata[0] > 0)     
            servovals[3] = indata[0];
          if(indata[1] > 0)
            servovals[4] = indata[1];
          writeServoVals();
          break;
        case 'c':
          payload[0] = comm[0];
          payload[1] = comm[1];
          Serial.println((char*)payload);
          sendMessage(3);
          break;
        case 'o':
          payload[0] = comm[0];
          payload[1] = comm[1];
          Serial.println((char*)payload);
          sendMessage(3);
          break;
        case 'm':
          payload[0] = comm[0];
          payload[1] = comm[1];
          Serial.println((char*)payload);
          sendMessage(3);
          break;
      }
      break;
  }
  Serial.print("Command Received: ");
  Serial.println(comm);
  for(int i = 0; i < 5; i++){
    Serial.print(i);
    Serial.print(": ");
    Serial.println(indata[i]);
  }
}

void writeServoVals(){
  memset(payload,0,sizeof(payload));
  for(int i = 0; i < 5; i++){
    payload[i] = servovals[i];
    Serial.println(servovals[i]);
  }
  Serial.println((char*)payload);
  sendMessage(2);
}

static void sendMessage(int dstEndpointVal){
  //Assigns values to the static data request struct
  nwkDataReq.dstAddr = 1;
  nwkDataReq.dstEndpoint = dstEndpointVal;
  nwkDataReq.srcEndpoint = 1;
  nwkDataReq.options = 0;
  nwkDataReq.data = (uint8_t*)payload;
  nwkDataReq.size = sizeof(payload);
  nwkDataReq.confirm = appDataConf;
  NWK_DataReq(&nwkDataReq);
}

static bool receiveMessage(NWK_DataInd_t *ind) {
  if(debug){
    Serial.print("Received message - ");
    Serial.print("lqi: ");
    Serial.print(ind->lqi, DEC);
  
    Serial.print("  ");
  
    Serial.print("rssi: ");
    Serial.print(ind->rssi, DEC);
    Serial.print("  ");
    Serial.print("Data: ");
    Serial.println(ind->size);
    Serial.print("message: ");
  }
  rec_message = (uint8_t*)(ind->data);
  switch((char)rec_message[0]){
    case 's':
      for(int i = 0; i < ind->size; i++){
        Serial.println((int)rec_message[i+1]);
      }
      break;
    case 'c':
      byte tmparr[4];
      float tmpflt;
      for(int i = 0; i < 4; i++){
        tmparr[i] = rec_message[i+1];
      }
      tmpflt = *(float *)&tmparr;
      Serial.print(tmpflt);
      Serial.print(",");
      for(int i = 0; i < 4; i++){
        tmparr[i] = rec_message[5+i];
      }
      tmpflt = *(float *)&tmparr;
      Serial.println(tmpflt);
      //Serial.print(",");
      break;
  }
  if((char)rec_message[0] == 's'){
    for(int i = 0; i < 5; i++){
      Serial.println((int)rec_message[i+1]);
    }
  }
  return true;
}