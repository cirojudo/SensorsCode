#include "OTABlynkCredentials.h"
#include <ESP8266HTTPClient.h>
#define SERVER_URL "wseeds.klaus.market"

#ifdef ESP8266
#include <BlynkSimpleEsp8266.h>
#elif defined(ESP32)
#include <BlynkSimpleEsp32.h>
#else
#error "Board not found"
#endif

credentials Credentials;



//Variables
char auth_token[33];
bool connected_to_internet = 0;
const int Erasing_button = 0;


//Provide credentials for your ESP server
char* esp_ssid = "wseedsolutions";
char* esp_pass = "wseedsolutions";

//
//
// .     Add your variables 
//
// .             HERE
// .
//
//





void setup()

{

  Serial.begin(115200);
  pinMode(Erasing_button, INPUT);


  for (uint8_t t = 4; t > 0; t--) {
    Serial.println(t);
    delay(1000);
  }
// TO DELETE PROM BY DEFAULT
// Credentials.Erase_eeprom();
  // Press and hold the button to erase all the credentials
  if (digitalRead(Erasing_button) == LOW)
  {
    Credentials.Erase_eeprom();

  }

  String auth_string = Credentials.EEPROM_Config();
  auth_string.toCharArray(auth_token, 33);

  if (Credentials.credentials_get())
  {

    //Blynk.config(auth_token);
    connected_to_internet = 1;

  }
  else
  {
    Credentials.setupAP(esp_ssid, esp_pass);
    connected_to_internet = 0;
  }


  if (connected_to_internet)
  {

    //
    //
    // .  Write the setup part of your code
    //
    // .             HERE
    // .
    //
    //

  }



}



void loop()
{
  Credentials.server_loops();

  if (connected_to_internet)
  {

delay(60*1000);
    WiFiClient client;
    HTTPClient http;

    //USE_SERIAL.print("[HTTP] begin...\n");
    DynamicJsonDocument doc(250);
    float L0 = 0.0;
    float L1 = 0.0;
    float T = 0.0;
    float H = 0.0;
    float A = 0.0;
    float P = 0.0;
   
    //String S = "";
    doc["type"] = "request";
    serializeJson(doc,Serial);
    // Reading the response
    boolean messageReady = false;
    String message = "";
    while(messageReady == false) { // blocking but that's ok
    if(Serial.available()) {
      message = Serial.readString();
      messageReady = true;
    }
  }
  DeserializationError error = deserializeJson(doc,message);
  if(error) {
    Serial.print(F("deserializeJson() wifi module failed: "));
    Serial.println(error.c_str());
    Serial.println(message);
    return;
  }
   http.begin( "http://" SERVER_URL "/measurements/wifi_module/");
       http.addHeader("Content-Type", "application/json");

    //USE_SERIAL.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    L0 = doc["L0"];
    L1 = doc["L1"];
    T = doc["T"];
    H = doc["H"];
    A = doc["A"];
    P = doc["P"];

    String json;
    serializeJson(doc, json);
    int httpCode = http.POST(json);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        //USE_SERIAL.println("received payload:\n<<");
        //USE_SERIAL.println(payload);
        //USE_SERIAL.println(">>");
      }
    } else {
      //USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}
