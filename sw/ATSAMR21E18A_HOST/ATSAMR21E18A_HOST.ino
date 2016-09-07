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

//Update 
static bool sendStatus(NWK_DataInd_t *ind);
static bool updateMotors(NWK_DataInd_t *ind);

//1 is MOJO control
//2 is USB bridge
int meshAddress = 1;



//Static data packet types (prevents memory leak)
//LWM data request struct
static NWK_DataReq_t nwkDataReq;
//Payload array (10 byte limit right now)
static uint8_t payload[9];
static uint8_t servovals[5];

uint8_t* rec_message;

byte pingCounter = 0;

boolean ledstatus = true; //for debug
long curtime = 0;
long ledupdatetime = 0;



void setup() {
  Serial.begin(115200);
  Serial.println("MOJO Control Board - no Tiny841");

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
  NWK_OpenEndpoint(2, updateMotors);
}

void loop() {
  SYS_TaskHandler();
  curtime = millis();  

  if(curtime - ledupdatetime > 10){
    ledupdatetime = curtime;
    digitalWrite(0, ledstatus);
  }
}

void parseMessage(char *data){
  if(data[0] == 'l'){
    ledstatus = !ledstatus;
  }
}

static void sendMessage(void) {
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
  Serial.print("Received message - ");
  Serial.print("lqi: ");
  Serial.print(ind->lqi, DEC);

  Serial.print("  ");

  Serial.print("rssi: ");
  Serial.print(ind->rssi, DEC);
  Serial.print("  ");
  payload[0]= 's';

  for(int i = 0; i < 5; i++){
    payload[i+1] = servovals[i];
  }

  sendMessage();

  ledstatus = true;  
  return true;
}

static bool updateMotors(NWK_DataInd_t *ind) {
  Serial.print("Received message - ");
  Serial.print("lqi: ");
  Serial.print(ind->lqi, DEC);

  Serial.print("  ");

  Serial.print("rssi: ");
  Serial.print(ind->rssi, DEC);
  Serial.print("  ");

  rec_message = (uint8_t*)(ind->data);

  Serial.println((char*)rec_message);

  for(int i = 0; i < 5; i++){
    servovals[i] = rec_message[i];
  }
  
  return true;
}
