
#include <ArduinoJson.h>

#define Serial SERIAL_PORT_USBVIRTUAL


//Object we're expecting
//const char* json = "{\"updates\":true,\"power\":[true,true],\"motorvals\":[126,125,221,32,180]}";

const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(3) + 70;

bool top_pow,bot_pow,updates;

static uint8_t servovals[5] = {0,0,0,0,0};

void setup() {
  Serial.begin(115200);
  top_pow = false; 
  bot_pow = false;
  updates = false;
}

void loop() {
  //Reads a command from Serial if available
  if(Serial.available() > 0){
    parseCommand();
  }

}

void parseCommand(){
  char json[255];
  char output[255];
  
  Serial.readBytesUntil(';',json,255);
  
  StaticJsonBuffer<bufferSize> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  
  JsonArray& motorvals = root["motorvals"];
  for(int i = 0; i < 5; i++){
    servovals[i] = (uint8_t)motorvals[i];
  }
  top_pow = root["power"][0];
  bot_pow = root["power"][1];
  updates = root["updates"];

  root.printTo(output,sizeof(output));
  Serial.write(output);
}

