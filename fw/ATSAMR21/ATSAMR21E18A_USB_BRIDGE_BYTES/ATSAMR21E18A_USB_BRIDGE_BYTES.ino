#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

//Atmel LWM Includes
#include "lwm.h"
#include "lwm/sys/sys.h"
#include "lwm/nwk/nwk.h"

//Makes definition easier
#define Serial SerialUSB
#define LED_BUILTIN 0

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
//Array to hold the message being read in 
static uint8_t inmess[255];

//MotorLayout is: Top Outer - Top Inner - Hip - Bot Outer Bot Inner
static uint8_t servovals[5] = {74,182,185,74,182};
static uint8_t config_byte = 0x00;

//Timing variables
unsigned long curtime = 0;
unsigned long syncstatetime, inctime;
unsigned long syncstateinterval = 100000;
unsigned long ledinterval = 500000;

//for debug
boolean ledstatus = true; 
boolean sync = false;
boolean debug = false;
boolean waiting = false;
long ledtoggletime = 0;

long timestep = 0;

long before, after;


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

  pinMode(LED_BUILTIN, OUTPUT);
  
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
  curtime = micros();

  //Reads a command from Serial if available
  if(Serial.available() > 0){
    parseCommand();
  }

  //Toggle LED stuff. More debug.
  if(curtime - ledtoggletime > ledinterval){
    ledtoggletime = curtime;
    ledstatus = !ledstatus;
    digitalWrite(LED_BUILTIN, ledstatus);
  }

  if(curtime - syncstatetime > syncstateinterval && sync){
    syncstatetime = curtime;
    sendState();
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
    if(debug)
      Serial.println("Sent successfully");
  else
    if(debug)
      Serial.println("Packet failed to send"); 
}

void parseCommand(){
  //clear the message 
  memset(payload,0,sizeof(payload));
  memset(inmess,0,sizeof(inmess));

  //Read the incoming message
  Serial.readBytesUntil(';',inmess,255);

  if(debug){
    Serial.print("Bridge Received: ");
    Serial.println((char*)inmess);
  }
  
  //The first two bytes are the message header
  //The first byte is the destination
  //The second byte is the command
  switch(inmess[0]){
    case 'w': //Messages meant for the coordinator
      switch(inmess[1]){
        case 's': //The message contains a full state
          //Right now not checking the packet properties
          memset(payload, 0, sizeof(payload));
          for(int i = 0; i < 6; i++){
            payload[i] = inmess[i+2];
          }
          sendMessage(2);
          break;
        case 'q':
          if(debug){
            SerialUSB.println("Requesting Coordinator State...");
          }
          sendMessage(1);
          break;
            
      }
      break;
    case '!': //Messages meant for this 
      switch(inmess[1]){
        case 'l':
          if(ledinterval == 500000)
            ledinterval = 100000;
          else
            ledinterval = 500000;
          break;
        case 'v':
          debug = !debug;
          break;      
      }
      break;
  }
}


void sendState(){
  memset(payload,0,sizeof(payload));
  memcpy(&payload, &servovals, sizeof(servovals));
  payload[5] = config_byte;
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
  //Network debug code
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
  //Assign a pointer to received message data
  rec_message = (uint8_t*) ind->data;

  Serial.write("Received data from coordinator: ");
  Serial.write((char*)rec_message);
  Serial.write("\n");
  
  //More debug
  if(debug){
    after = micros();
    for(int i = 0; i < 5; i++){
      Serial.print(rec_message[i],DEC);
      Serial.print(" | ");
    }
    Serial.println();
    SerialUSB.println("INA3221 A:");
    for(int i = 0; i < 5; i++){
      SerialUSB.print("\tChannel ");
      SerialUSB.print(i+1);
      SerialUSB.println(":");
      SerialUSB.print("\t\tV: ");
      SerialUSB.print(0.001*((rec_message[i*2+16] << 8) | rec_message[i*2+17]));
      SerialUSB.println("V");
      SerialUSB.print("\t\tC: ");
      SerialUSB.print(0.16*((rec_message[i*2+6] << 8) | rec_message[i*2+7]));
      SerialUSB.println("mA");
    }
    Serial.println("Battery A");
    Serial.print("\tSoC: ");
    Serial.print(1.0/256.0*((rec_message[26]<<8) | rec_message[27]));
    SerialUSB.println("%");
    Serial.print("Time Elapsed: ");
    Serial.println(after-before);
  }
  return true;
}
