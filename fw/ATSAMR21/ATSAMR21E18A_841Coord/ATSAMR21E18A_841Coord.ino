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
static uint8_t payload[9];
static uint8_t indata[5];

// 
// Interpolation patterns for the keyframes in the locomotion algorithm
// Each can have a maximum of 8th degree
// Points are between f(x) = coeff[j][0]*x^8 + ... + coeff[j][7]
// Where x is between 0 and 127, and j is the frame number
// 0 - 1 - 2 - 3 - 4 - 5 - 6 - 0 ...
//
static int coeff[7][8];

//Flag to set either linear interpolation or polynomial interpolation between keyframes
boolean lin_interp = true;

int framecounter = 0;
static int translation_keyframes[16][2] = {{6,6},
                                           {6,5},
                                           {6,4},
                                           {6,3},
                                           {0,3},
                                           {2,3},
                                           {1,3},
                                           {1,2},
                                           {1,1},
                                           {2,1},
                                           {3,1},
                                           {4,1},
                                           {4,0},
                                           {4,5},
                                           {4,6},
                                           {5,6}};
/*                               
 * These were designed for a milled delrin armset
 *
static int framevals[7][2] = {{100, 150},  // 0
                              {180, 200},  // 1
                              {180, 210},  // 2
                              { 60, 210},  // 3
                              { 20, 180},  // 4
                              { 20,  60},  // 5
                              { 60,  60}}; // 6 
* 
* SCHEMATIC REPRESENTATION OF MOJO
* 
*              5 - 6
*   |----| /  /     \
*   |  O |/  4       \
*   |    |   |        0
*   | O  |\  3       /
*   |____| \  \     / 
*     HH       2 - 1
*     
*     
* These were chosen for the injection molded armset    
*/
static int framevals[7][2] = {{100, 150},  // 0
                              {180, 240},  // 1
                              {180, 240},  // 2
                              { 30, 240},  // 3
                              { 20, 180},  // 4
                              { 20,  90},  // 5
                              { 60,  90}}; // 6
 /*                             
static int hipframevals[10][5] = {{60,90,175,60,90},
                                 {60,90,175,30,180},
                                 {100,150,175,30,180},
                                 {100,150,1,30,180},
                                 {60,90,1,30,180},
                                 {60,90,1,30,90},
                                 {60,90,1,60,90},
                                 {30,200,1,60,90},
                                 {30,200,1,100,150},
                                 {30,200,175,100,150},
                                 {30,200,175,60,90},
                                 {30,90,175,60,90},
                                 {60,90,175,60,90}};
                                 */
static uint8_t servovals[5];
static uint8_t servo_corrections[5] = {47,-8,25,17,-48};
static uint8_t tarservovals[5];

uint8_t* rec_message;


boolean ledstatus = true; //for debug
boolean moving = false;
boolean hipmoving = false;
boolean currentping = false;
boolean i2c_ready = false;

/* TIMING VARIABLES
 *  
 */
long curtime = 0;
long ledupdatetime = 0;
long servoupdatetime = 0;
long currentreadtime = 0;
long currentsendtime = 0;
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

  tarservovals[0] = servovals[0];
  tarservovals[1] = servovals[1];
  tarservovals[2] = servovals[2];
  tarservovals[3] = servovals[3];
  tarservovals[4] = servovals[4];
}

void loop() {
  //Wireless comms communication
  SYS_TaskHandler();
  curtime = millis();  

  if(Serial.available() > 0)
    parseSerialMessage();
  
  
  if(curtime - ledupdatetime > 50){
    //ledstatus = !ledstatus;
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

  if(curtime - currentsendtime > 100 && currentping){
    currentsendtime = curtime;
    sendCurrentValues();
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
        case 'm':
          moving = !moving;
          break;
        case 'h':
          hipmoving = !hipmoving;
          break;
        case 'p':
          currentping = !currentping;
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
    case 's':
       if(usingSerial){
          Serial.println("Status:");
          for(int i = 0; i < 5; i++){
            Serial.print("Servo ");
            Serial.print(i+1);
            Serial.print(": ");
            Serial.println(servovals[i]);
          }
        }else{
          payload[0]= 's';
          for(int i = 0; i < 5; i++){
            payload[i+1] = servovals[i];
          }
          sendMessage();
        }
      break;
    case 'c':
      readCurrentValues();
      if(comm[1] == 'p')
        currentping = !currentping;
      break;
    case 'w':
      switch(comm[1]){
        case 't':
          if(indata[0] > 0)
            tarservovals[0] = indata[0];
          if(indata[1] > 0)
            tarservovals[1] = indata[1];
          break;
        case 'h':        
          if(indata[0] > 0)      
            tarservovals[2] = indata[0];
          break;
        case 'b': 
          if(indata[0] > 0)     
            tarservovals[3] = indata[0];
          if(indata[1] > 0)
            tarservovals[4] = indata[1];
          break;
        case 'c':
          tarservovals[0] = 60;
          tarservovals[1] = 90;
          tarservovals[3] = 60;
          tarservovals[4] = 90;
          break;  
        case 'o':
          tarservovals[0] = 30;
          tarservovals[1] = 200;
          tarservovals[3] = 30;
          tarservovals[4] = 200;
          break;
        case 'm':
          moving = true;
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
    bool nextframe = true;

    for(int i = 0; i < 5; i++)
      if(tarservovals[i] - servovals[i] != 0 )
        nextframe = false;
      
    if(nextframe && moving && curtime - nextframetime > nextframeinterval){
      nextframetime = curtime;
      framecounter = (framecounter+1)%16;
      if(framecounter == 0)
        moving = false;
      tarservovals[0] = framevals[translation_keyframes[framecounter][1]][0];
      tarservovals[1] = framevals[translation_keyframes[framecounter][1]][1];
      tarservovals[3] = framevals[translation_keyframes[framecounter][0]][0];
      tarservovals[4] = framevals[translation_keyframes[framecounter][0]][1];
    }

    if(nextframe && hipmoving && curtime - nextframetime > nextframeinterval){
      nextframetime = curtime;
      framecounter = (framecounter+1)%10;
      if(framecounter == 0)
        hipmoving = false;
      //for(int i = 0; i < 5; i++)
        //tarservovals[i] = hipframevals[framecounter][i];
    }
    
    /*if(!moving){*/
      for(int i = 0; i < 5; i++){
        if(tarservovals[i] - servovals[i] > 0)
          servovals[i]++;
        else if(tarservovals[i] - servovals[i] < 0)
          servovals[i]--;
      }
    /*}*/
    

    Wire.beginTransmission(4);
      Wire.write(servovals[0]+servo_corrections[0]);
      Wire.write(servovals[1]+servo_corrections[1]);
      Wire.write(servovals[2]+servo_corrections[2]);
    Wire.endTransmission();
    
    Wire.beginTransmission(2);
      Wire.write(servovals[3]+servo_corrections[3]);
      Wire.write(servovals[4]+servo_corrections[4]);
      Wire.write(servovals[2]+servo_corrections[2]);
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
      tarservovals[i] = rec_message[i];
  
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

