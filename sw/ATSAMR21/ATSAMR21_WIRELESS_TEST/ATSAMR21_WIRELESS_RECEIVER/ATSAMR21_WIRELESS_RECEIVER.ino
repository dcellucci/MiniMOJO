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

//1 is Receiver
//2 is USB bridge
int meshAddress = 1;

char payload[64] = "|abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
int curloc = 0;
//Static data packet types (prevents memory leak)
//LWM data request struct
static NWK_DataReq_t nwkDataReq;
//Payload array (10 byte limit right now)

uint8_t* rec_message;

byte pingCounter = 0;

boolean ledstatus = true; //for debug
boolean debugmode = false;
long curtime = 0;
long ledupdatetime = 0;
long servoupdatetime = 0;
long currentreadtime = 0;


void setup() {
  if(debugmode){
    Serial.begin(115200);
    Serial.println("MOJO Control Board - Receiver");
  }

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

}

void loop() {
  SYS_TaskHandler();
}

void parseMessage(char *data){
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
  if(debugmode){
    if (NWK_SUCCESS_STATUS == req->status)
      Serial.println("Sent successfully");
    else
      Serial.println("Packet failed to send");
  }
}

static bool sendStatus(NWK_DataInd_t *ind) {
  if(debugmode){
    Serial.print("Status Request Received...");
    Serial.print("lqi: ");
    Serial.print(ind->lqi, DEC);
  
    Serial.print("  ");
  
    Serial.print("rssi: ");
    Serial.print(ind->rssi, DEC);
    Serial.print("  ");
  }

  char tmp = payload[(curloc+1)%63];
  payload[(curloc+1)%63]='|';
  payload[curloc]=tmp;
  sendMessage();
  curloc = (curloc+1)%63;

  return true;
}

