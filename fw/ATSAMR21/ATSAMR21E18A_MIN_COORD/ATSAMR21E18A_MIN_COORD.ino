#include <Arduino.h>

//Comms with the sensors/control boards
#include <Wire.h>

//Comms with the onboard RF transceiver
#include <SPI.h>

//Atmel lightweight mesh
#include "lwm.h"
#include "lwm/sys/sys.h"
#include "lwm/nwk/nwk.h"

//IMU sensor
#include <SparkFunLSM9DS1.h>

//Serial USB because I always forget
#define Serial SerialUSB


//
// LSM9DS1 Settings
//

// SDO_M and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION 6.24 // Declination (degrees) in Sanford, FL.

extern "C" {
  void println(char *x) { Serial.println(x); Serial.flush(); }
}

//Instantiate an IMU object
LSM9DS1 imu;

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
static bool updateState(NWK_DataInd_t *ind);
//Alters Specific Settings
static bool changeSetting(NWK_DataInd_t *ind);

//Are we using serial comms?
boolean usingSerial = true;

//1 is MOJO control
//2 is USB bridge
int meshAddress = 1;

//0x01 default
//0x02 alternate
int panID = 0x02;

//Static data packet types (prevents memory leak)
//LWM data request struct
static NWK_DataReq_t nwkDataReq;

//Payload array (30 byte limit right now)
static uint8_t payload[50];
static uint8_t indata[5];
                                 
static uint8_t servovals[5];
static uint8_t trimvals[5] = {7,-13,7,-13,5};

uint8_t* rec_message;

boolean debug = false;
boolean ledstatus = true; //for debug

boolean currentping = false;
boolean imu_works = true;

boolean top_servopower = true;
boolean bot_servopower = true;

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
  NWK_SetPanId(panID);
  PHY_SetChannel(0x1a);
  PHY_SetRxState(true);
  NWK_OpenEndpoint(1, sendStatus);
  NWK_OpenEndpoint(2, updateState);
  NWK_OpenEndpoint(3, changeSetting);
  pinMode(0,OUTPUT);

  Wire.begin();

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  if (!imu.begin())
  {
    if(debug){
      Serial.println("Failed to communicate with LSM9DS1.");
      Serial.println("Double-check wiring.");
      Serial.println("Default settings in this sketch will " \
                    "work for an out of the box LSM9DS1 " \
                    "Breakout, but may need to be modified " \
                    "if the board jumpers are.");
    }
    imu_works = false;
  }

  //Initialize Servo Values to safe initial conditions
  servovals[0] = 128;
  servovals[1] = 128;
  servovals[2] = 128;
  servovals[3] = 184;
  servovals[4] = 180;

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

    if(!imu_works){
      imu_works = true;
      if(!imu.begin()){
        imu_works = false;
      }
    }
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
         case 'p':
          top_servopower = !top_servopower;
          bot_servopower = !bot_servopower;
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
  if(imu_works) {
    imu.readAccel();
    imu.readMag();
    imu.readGyro();
  }
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
  //populating the configuration register
  payload[5] = (imu_works << 3) | (top_servopower << 2) | (bot_servopower << 1); 
  //populating the servoval and current sensor registers
  for(int i = 0; i < 5; i++){ //The third channel of the bottom should be unused
    payload[i] = servovals[i];
    payload[i*2+6] = (vshunts[i]>>8) & 0xFF;
    payload[i*2+7] = (vshunts[i]) & 0xFF;
    payload[i*2+16] = (vbuses[i]>>8) & 0xFF;
    payload[i*2+17] = (vbuses[i]) & 0xFF;
  }
  //populating the state of charge registers
  payload[26] = (socs[0]>>8)&0xFF;
  payload[27] = (socs[0] & 0xFF);
  payload[28] = (socs[1]>>8)&0xFF;
  payload[29] = (socs[1] & 0xFF);

  if(imu_works){
    //populating the IMU register
    
    //the gyroscope
    payload[30] = (imu.gx>>8)&0xFF;
    payload[31] = (imu.gx)&0xFF;
    payload[32] = (imu.gy>>8)&0xFF;
    payload[33] = (imu.gy)&0xFF;
    payload[34] = (imu.gz>>8)&0xFF;
    payload[35] = (imu.gz)&0xFF;
    
    //accelerometer
    payload[36] = (imu.ax>>8)&0xFF;
    payload[37] = (imu.ax)&0xFF;
    payload[38] = (imu.ay>>8)&0xFF;
    payload[39] = (imu.ay)&0xFF;
    payload[40] = (imu.az>>8)&0xFF;
    payload[41] = (imu.az)&0xFF;
    
    //magnetometer
    payload[42] = (imu.mx>>8)&0xFF;
    payload[43] = (imu.mx)&0xFF;
    payload[44] = (imu.my>>8)&0xFF;
    payload[45] = (imu.my)&0xFF;
    payload[46] = (imu.mz>>8)&0xFF;
    payload[47] = (imu.mz)&0xFF;
  }
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
      Wire.write(servovals[0]+trimvals[0]);
      Wire.write(servovals[1]+trimvals[1]);
      Wire.write(servovals[4]+trimvals[4]);
      if(top_servopower)
        Wire.write('+');
      else
        Wire.write('-');
    Wire.endTransmission();
    
    Wire.beginTransmission(bot_servo_ctrlr);
      Wire.write('w');
      Wire.write(servovals[2]+trimvals[2]);
      Wire.write(servovals[3]+trimvals[3]);
      Wire.write(servovals[4]+trimvals[4]);
      if(bot_servopower)
        Wire.write('+');
      else
        Wire.write('-');
    Wire.endTransmission();
}

