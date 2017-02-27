#define Serial SERIAL_PORT_USBVIRTUAL

static uint8_t servovals[5] = {74,182,185,174,182};
static char BUFFER[255];
static uint8_t OUT[255];

void setup() {
  Serial.begin(115200);
}

void loop() {
  //Reads a command from Serial if available
  if(Serial.available() > 0){
    parseCommand();
  }

}

void parseCommand(){
  char comm[2];
  Serial.readBytesUntil(' ', comm, 3);
  if(comm[0] == 'w' && comm[1] == 'a'){
    memset(BUFFER,0,sizeof(BUFFER));
    memset(servovals,0,5);
    char servoval[3];
    for(int i = 0; i < 5; i++){
        if(Serial.available())
          servovals[i] = Serial.parseInt();
        else
          i = 5;
      }
    
      strcpy(BUFFER,"ma ");
     
      for(int i = 0; i < 5; i++){
        itoa(servovals[i],servoval,10);
        strcat(BUFFER,servoval);
        strcat(BUFFER," ");
      }
      strcat(BUFFER,"\n");
      Serial.write(BUFFER);  
  }
}

