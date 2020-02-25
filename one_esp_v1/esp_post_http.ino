/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
//#define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER_URL "api.knnabis.co"

#ifndef STASSID
#define STASSID "NonFreeWifi"
#define STAPSK  "167222791"
#endif

void setup() {

  USE_SERIAL.begin(9600);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    USE_SERIAL.print(".");
  }
  USE_SERIAL.println("");
  USE_SERIAL.print("Connected! IP address: ");
  USE_SERIAL.println(WiFi.localIP());

}

void loop() {
  // wait for WiFi connection
  //delay(3*60*1000);
  delay(10000);
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    //USE_SERIAL.print("[HTTP] begin...\n");
    DynamicJsonDocument doc(1024);
    int L = 0; 
    float T = 0;
    float H = 0;
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
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    Serial.println(message);
    return;
  }
  


    

    // configure traged server and url
    http.begin(client, "http://" SERVER_URL "/measurements/wifi_module/"); //HTTP
    http.addHeader("Content-Type", "application/json");

    //USE_SERIAL.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    L = doc["L"];
    T = doc["T"];
    H = doc["H"];
    String json;
    serializeJson(doc, json);
    int httpCode = http.POST(json);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);

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

  //delay(10000);
}