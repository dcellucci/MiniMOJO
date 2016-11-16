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
//Acknowledgement Char
char ack = 'o';

//Static data packet types (prevents memory leak)
//LWM data request struct
static NWK_DataReq_t nwkDataReq;
//Payload array (10 byte limit right now)
const int payload_size = 10;
static uint8_t payload[payload_size];
static uint8_t indata[5];

//Timing variables
long curtime = 0;

//for debug
boolean ledstatus = true; 
long ledtoggletime = 0;

boolean debug = false;


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
  if(curtime - ledtoggletime > 10){
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
  //Clear indata and payload structs
  memset(indata,0,sizeof(indata));
  memset(payload,0,sizeof(payload));
  int dex;
  if(Serial.available()){
    switch(Serial.read()){
      case 'l':
        ledstatus = !ledstatus;
        digitalWrite(0,ledstatus);
        break;
      case 'v':
        debug = !debug;
        if(debug)
          Serial.println("Debug Serial Enabled");
        break;
      case 's':
        //Status request packet, send to endpoint 1
        sendMessage(1);
        break;
      case 'w':
        for(int i = 0; i < 5; i++){
          payload[i] = Serial.read();
        }
        //Motor update packet, send to endpoint 2
        sendMessage(2);
        break;
      case '!':
        dex = 0;
        while(Serial.available() && dex < payload_size-1){
          payload[dex] = Serial.read();
          dex++;
        }
        if(debug){
          Serial.print("Received setting alteration command: ");
          Serial.println((char*)payload);
        }
        //Setting alteration command, send to endpoint 3
        sendMessage(3);
        break;
      case 'c':
        //Current Request Packet, send to endpoint 4
        sendMessage(4);
        break;
    }
    if(debug)
      Serial.print("Acknowledge: ");
    Serial.println(ack);
  }
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

// Parses a receive message
// Will give packet info if 'debug' is turned on
// Otherwise, outputs the info, and formats it if necessary

static bool receiveMessage(NWK_DataInd_t *ind) {
  rec_message = (uint8_t*)(ind->data);

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
    Serial.println("message: ");
    Serial.println((char*)rec_message);

  }
  if(rec_message[0] == 'c'){
    Serial.print("c,");
    byte tmparr[4];
    int tmpint;
    for(int i = 0; i < 4; i++){
      tmparr[i] = rec_message[i+1];
    }
    //gotta cast that byte array
    tmpint = (int)*(float *)&tmparr;
    Serial.print(tmpint);
    Serial.print(",");
    for(int i = 0; i < 4; i++){
      tmparr[i] = rec_message[5+i];
    }
    tmpint = (int)*(float *)&tmparr;
    Serial.println(tmpint);
  }
  else
    Serial.println((char*)rec_message);
  return true;
}
