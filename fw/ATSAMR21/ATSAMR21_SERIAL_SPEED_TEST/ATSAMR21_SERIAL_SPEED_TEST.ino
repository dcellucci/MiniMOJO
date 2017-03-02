
#include <ArduinoJson.h>

#define Serial SERIAL_PORT_USBVIRTUAL

#define bytearr
//Object we're expecting
//if JSON "{"updates":true,"power":[true,true],"motorvals":[126,125,221,32,180]};"
//if bytearr "wsABABo;"


#ifdef JSON
#include <ArduinoJson.h>

const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(3) + 70;
bool top_pow,bot_pow,updates;

static uint8_t servovals[5] = {0,0,0,0,0};
#endif

char BUFFER[255];

unsigned long before, after;

void setup() {
  Serial.begin(115200);
#ifdef JSON
  top_pow = false; 
  bot_pow = false;
  updates = false;
#endif
  before = 0;
  after = 0;
}

void loop() {
  //Reads a command from Serial if available
  if(Serial.available() > 0){

    parseCommand();
  }
  
}

void parseCommand(){
  before = micros();
#ifdef JSON
  char json[255];
  //char output[255];
  
  //Serial.readBytesUntil(';',json,255);
  
  StaticJsonBuffer<bufferSize> jsonBuffer;
  //JsonObject& root = jsonBuffer.parseObject(json);
  
  JsonObject& root = jsonBuffer.parse(Serial);
  
  JsonArray& motorvals = root["motorvals"];
  for(int i = 0; i < 5; i++){
    servovals[i] = (uint8_t)motorvals[i];
  }
  top_pow = root["power"][0];
  bot_pow = root["power"][1];
  updates = root["updates"];

  root.printTo(BUFFER,sizeof(BUFFER));
#endif

#ifdef bytearr
  Serial.readBytesUntil(';',BUFFER,sizeof(BUFFER));
#endif
  Serial.write(BUFFER);
  after = micros();
  Serial.print("Time it took: ");
  Serial.println(after-before);
}

