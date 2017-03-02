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

#define JSON
//#define DEBUG

#ifdef JSON
#include <ArduinoJson.h>

const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(3) + 70;
bool top_pow,bot_pow,updates;

static uint8_t servovals[5] = {0,0,0,0,0};
#endif

char BUFFER[255];

unsigned long before, after;

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


//Command input (from Serial)
String command = "";

//Timing variables
long curtime = 0;


//for debug
boolean ledstatus = true; 
long ledtoggletime = 0;


void setup() {
  Serial.begin(115200);
#ifdef JSON
  top_pow = false; 
  bot_pow = false;
  updates = false;
#endif
  before = 0;
  after = 0;

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
  
  //Reads a command from Serial if available
  if(Serial.available() > 0){
#ifdef DEBUG
    Serial.println("Received data");
#endif
    parseCommand();
  }
}

void parseCommand(){
  memset(payload, 0, sizeof(payload));
  before = micros();
#ifdef JSON
  char json[255];
  
  Serial.readBytesUntil(';',json,255);
  
  StaticJsonBuffer<bufferSize> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  //JsonObject& root = jsonBuffer.parse(Serial);
  
  JsonArray& motorvals = root["motorvals"];
  payload[0] = 'w';
  payload[1] = 's';
  for(int i = 0; i < 5; i++){
    payload[i+2] = (uint8_t)motorvals[i];
  }
  payload[7] = (((int)root["power"][0])<<3 | ((int)root["power"][1]) << 2 | ((int)root["updates"]));
  //Serial.println((char *)payload);
#endif

#ifdef bytearr
  Serial.readBytesUntil(';',payload,sizeof(payload));
#endif
  sendMessage(1);
  //Serial.write(BUFFER);
  //after = micros();
  //Serial.print("Time it took: ");
  //Serial.println(after-before);
}

/*
 * Was having trouble with transmission before I
 * added this confirmation method.
 * It would send a single packet over the network and 
 * then freeze... this method fixed it.
 */
static void appDataConf(NWK_DataReq_t *req){
#ifdef DEBUG
  if (NWK_SUCCESS_STATUS == req->status)
    Serial.println("Sent successfully");
  else
    Serial.println("Packet failed to send");
#endif
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
#ifdef DEBUG
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
#endif
  
  rec_message = (uint8_t*)(ind->data);
#ifdef DEBUG
  Serial.println("messaged received from transmitter")
  Serial.write((char*)rec_message);
#endif
#ifdef JSON
  StaticJsonBuffer<bufferSize> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["updates"] = (bool)(rec_message[7] & 0x01);
  JsonArray& power = root.createNestedArray("power");
  power.add((bool)(rec_message[7] & 0x04));
  power.add((bool)(rec_message[7] & 0x02));
  JsonArray& motorVals = root.createNestedArray("motorvals");
  motorVals.add(rec_message[2]);
  motorVals.add(rec_message[3]);
  motorVals.add(rec_message[4]);
  motorVals.add(rec_message[5]);
  motorVals.add(rec_message[6]);
  
  root.printTo(BUFFER,sizeof(BUFFER));
  Serial.write(BUFFER);
#endif

#ifdef bytearr
  Serial.print((char*)rec_message);
  Serial.print(" ");
#endif
  after = micros();
  //Serial.print("Time it took: ");
  Serial.println(after-before);


  return true;
}
