#define Serial SerialUSB

int count;
char outbuf[5];
char inbuf[100];
unsigned long curtime, pingtime;
unsigned long pinginterval = 10;
bool waitingForPing;

unsigned long timepingsent, timepingrec;

//Testing network latency for the SERIAL PORT
//Bridge -> Websockets Host -> Websockets Client -> Host -> Bridge 
#define SER_PING
#define 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
#ifdef SER_PING
  count = 0;
  waitingForPing = false;
#endif

}

void loop() {
  curtime = millis();
  
#ifdef SER_PING
  if(curtime - pingtime > pinginterval && !waitingForPing){
    pingtime = curtime;
    waitingForPing = true;
    sprintf(outbuf,"!p%05d",count);
    //timestamp current time
    timepingsent = micros();
    Serial.println(outbuf);
    count++;
  }
  if(curtime - pingtime > pinginterval * 10){
    waitingForPing = false;
  }
#endif

  if(Serial.available() > 0){
    memset(inbuf,0,sizeof(inbuf));
    Serial.readBytesUntil(';',inbuf,25);
    if(inbuf[0] == 'e'){
      pingtime = curtime;
      waitingForPing = false;
      timepingrec = micros();
      memset(outbuf, 0, sizeof(outbuf));
      sprintf(outbuf,"l%d",timepingrec-timepingsent);
      Serial.println(outbuf);
    }
  }
}
