#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>

//Atmel LWM Includes
#include "lwm.h"
#include "lwm/sys/sys.h"
#include "lwm/nwk/nwk.h"

//Makes definition easier
#define Serial SERIAL_PORT_USBVIRTUAL

extern "C" {
  void println(char *x) { Serial.println(x); Serial.flush(); }
}

//ArduinoJson 
const size_t bufferSize = 2*JSON_ARRAY_SIZE(2) + 3*JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(5) + 150;
char BUFFER[255]; 

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

static uint8_t servovals[5] = {74,182,185,174,182};
static uint8_t config_byte = 0x04;

//Command input (from Serial)
String command = "";

//Timing variables
unsigned long curtime = 0;
unsigned long syncstatetime, inctime;
unsigned long syncstateinterval = 15000;

//for debug
boolean ledstatus = true; 
boolean sync = false;
boolean debug = false;
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

  pinMode(0, OUTPUT);
  
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
  if(curtime - ledtoggletime > 500000){
    ledtoggletime = curtime;
    ledstatus = !ledstatus;
    digitalWrite(0, ledstatus);
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
  if (NWK_SUCCESS_STATUS == req->status && debug){
    Serial.println("Sent successfully");
  }
  else{
    Serial.println("Packet failed to send"); 
  }
}

void parseCommand(){
  memset(payload,0,sizeof(payload));
  
  char json[255];
  Serial.readBytesUntil(';',json,255);
  
  
  if(json[0] == '{'){
    StaticJsonBuffer<bufferSize> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    JsonArray& motorvals = root["motorvals"];
    for(int i = 0; i < 5; i++){
      servovals[i] = (uint8_t)motorvals[i];
    }
    config_byte = (((int)root["power"][0])<<3 | ((int)root["power"][1]) << 2 | ((int)root["updates"]));
    sendState();
  }
  if(json[0] == 's'){
    if(debug)
      before = micros();
    sendMessage(1);
  }
  /*
  else{
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
          case 'a':
            for(int i = 0; i < 5; i++)
              if(indata[i] > 0)
                servovals[i] = indata[i];
            writeServoVals();
            break;
          case '+':
          case '-':
          case '_':
          case '=':
          case 'c':
          case 'o':
          case 'm':
            payload[0] = comm[0];
            payload[1] = comm[1];
            Serial.println((char*)payload);
            sendMessage(3);
            break;
        }
        break;
      }
      
  }
  */
  /*
  Serial.print("Command Received: ");
  Serial.println(comm);
  if(comm[0] == 'w'){
    for(int i = 0; i < 5; i++){
      Serial.print(i);
      Serial.print(": ");
      Serial.println(indata[i]);
    }
  }
  */
}

void sendState(){
  memset(payload,0,sizeof(payload));
  memcpy(&payload, &servovals, sizeof(servovals));
  payload[5] = config_byte;
  
  sendMessage(2);
  sync = false;
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
  
  StaticJsonBuffer<bufferSize> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& power = root.createNestedArray("power");
  power.add((bool)(rec_message[5] & 0x04));
  power.add((bool)(rec_message[5] & 0x02));
  JsonArray& motorVals = root.createNestedArray("motorvals");
  JsonArray&      vbus = root.createNestedArray("vbus");
  JsonArray&    vshunt = root.createNestedArray("vshunt");
  for(int i = 0; i < 5; i++){
    motorVals.add(rec_message[i]);
    vbus.add(((rec_message[i*2+16] << 8) | rec_message[i*2+17]));
    vshunt.add(((rec_message[i*2+6] << 8) | rec_message[i*2+7]));
  }
  JsonArray& soc = root.createNestedArray("soc");
  soc.add(((rec_message[26]<<8) | rec_message[27]));
  soc.add(((rec_message[28]<<8) | rec_message[29]));

  root.printTo(BUFFER,sizeof(BUFFER));
  Serial.write(BUFFER);
  
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
      SerialUSB.print(4.0*0.04*((rec_message[i*2+6] << 8) | rec_message[i*2+7]));
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
