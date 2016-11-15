#include <Arduino.h>
#include <Wire.h>

#include <SPI.h>

#define Serial SERIAL_PORT_USBVIRTUAL

#include "lwm.h"
#include "lwm/sys/sys.h"
#include "lwm/nwk/nwk.h"

#include <MOJO_INA219.h>

extern "C" {
  void println(char *x) { Serial.println(x); Serial.flush(); }
}

#define LED_PIN 0
 
// LWM mesh methods 

//Send the message
static void sendMessage();
//Checks for confirmation (freezes otherwise)
static void appDataConf(NWK_DataReq_t *req);
//
//Wireless Comms Functions 
//
// Each is configured to an endpoint, such that
// a packet with that endpoint destination calls 
// that method when parsed.

//Sends Current Status, single packet
//Endpoint: 1
//Payload:  None
static bool sendStatus(NWK_DataInd_t *ind);

//Updates the motor positions
//Endpoint: 2
//Payload:  [bbbbb]
static bool updateMotors(NWK_DataInd_t *ind);

//Alters Specific Settings
//Endpoint: 3
//Payload:  None
static bool changeSetting(NWK_DataInd_t *ind);

//Are we using serial comms?
boolean usingSerial = false;

//1 is MOJO control
//2 is USB bridge
int meshAddress = 1;

//Static data packet types (prevents memory leak)
//LWM data request struct
static NWK_DataReq_t nwkDataReq;

//Payload array (10 byte limit right now)
static uint8_t payload[9];
static uint8_t indata[5];

//
uint8_t* rec_message;


//Booleans
boolean ledstatus = true; //for debug
boolean i2c_ready = false;

/* TIMING VARIABLES
 *  
 */
long curtime = 0;
long ledupdatetime = 0;
long servoupdatetime = 0;
long currentreadtime = 0;
long nextframetime = 0;

int nextframeinterval = 1000;

/*I2C Settings
 * 
 */
uint8_t top_servo_address = 0x04;
uint8_t bot_servo_address = 0x02;
uint8_t top_cur_address = 0x45;
uint8_t bot_cur_address = 0x40;


Adafruit_INA219 top_cur_sen,bot_cur_sen;
float top_cur_mA = 0;
float bot_cur_mA = 0;
float correction_factor = 1.375;

int servovals[5];

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
  NWK_OpenEndpoint(2, updateMotors);
  NWK_OpenEndpoint(3, changeSetting);

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
  curtime = millis();  

  if(Serial.available() > 0)
    parseSerialMessage();
  
  
  if(curtime - ledupdatetime > 1000){
    ledstatus = !ledstatus;
    ledupdatetime = curtime;
    digitalWrite(0, ledstatus);
  }
  
  
  if(curtime - servoupdatetime > 10 && i2c_ready){
    servoupdatetime = curtime;
    updateServos();
  }

  if(curtime - currentreadtime > 100 && i2c_ready){
    currentreadtime = curtime;
    readCurrentValues();
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
          usingSerial = !usingSerial;
          break;
        case 'i':
          i2c_startup();
          break;
        case 'I':
          usingSerial = !usingSerial;
          i2c_startup();
          break;
        case 'l':
          ledstatus = !ledstatus;
          break;
      }
      break;
   }
}

void readCurrentValues(){
  currentreadtime = curtime;
  top_cur_mA = top_cur_sen.getCurrent_mA()*correction_factor;
  bot_cur_mA = bot_cur_sen.getCurrent_mA()*correction_factor;
}

void sendCurrentValues(){
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
  }
}

void updateServos(){   
    Wire.beginTransmission(4);
      Wire.write(servovals[0]);
      Wire.write(servovals[1]);
      Wire.write(servovals[2]);
    Wire.endTransmission();
    
    Wire.beginTransmission(2);
      Wire.write(servovals[3]);
      Wire.write(servovals[4]);
      Wire.write(servovals[2]);
    Wire.endTransmission();
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
  if(usingSerial){
    Serial.print("Received message - ");
    Serial.print("lqi: ");
    Serial.print(ind->lqi, DEC);
  
    Serial.print("  ");
  
    Serial.print("rssi: ");
    Serial.print(ind->rssi, DEC);
    Serial.print("  ");
  }
  payload[0]= 's';

  for(int i = 0; i < 5; i++){
    payload[i+1] = servovals[i];
  }

  sendMessage();

  ledstatus = true;  
  return true;
}

static bool updateMotors(NWK_DataInd_t *ind) {
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
  
  return true;
}

static bool changeSetting(NWK_DataInd_t *ind){
  rec_message = (uint8_t*)(ind->data);
  parseCommand((char*)rec_message);
  return true;
}

void i2c_startup(){
  top_cur_sen.begin(top_cur_address);
  top_cur_sen.setCalibration_MOJO();
  bot_cur_sen.begin(bot_cur_address);
  bot_cur_sen.setCalibration_MOJO();

  i2c_ready = true;
}

