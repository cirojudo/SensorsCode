//////////////////////////////////////////////////////
////////////////// WSEEDSOLUTIONS ////////////////////
//////////////////////////////////////////////////////
////////////// Analog Photoresistor //////////////////
//////////////////////////////////////////////////////
////////////////// Ligth sensor //////////////////////
/////////////////////  (2)  //////////////////////////
//////////////////////////////////////////////////////
///////////// Temperature - Humidity /////////////////
//////////////////////////////////////////////////////
////////////// Arduino Sensor code ///////////////////
//////////////////////////////////////////////////////

#include <ArduinoJson.h>

//////////////////////
////Serial device/////
//////////////////////

String serialDevice = "w_4";

//////////////////////

//comunication
String message = "";
bool messageReady = false;

int voltageread = 0;  //Voltage reading for lux conversion

//////////////// Code for DHT 11 sensor //////////////

#include "DHT.h"
#define DHTPIN 2     // Digital pin 2 
#define DHTTYPE DHT22   // DHT 11 sensor

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino


//Method for voltage to lux conversion//
double Light (int voltageread)
{
    double Vout = voltageread * 0.0048828125; // Voltage output
    //int lux = (2500 / Vout - 500) / 10;
     int lux=500/(10*((5-Vout)/Vout)); //Conversion to Lux
    return lux;
}

//Method 2 for voltage to lux conversion//
double Light2 (int voltageread)
{
    double Vout = voltageread * 0.0048828125; // Voltage output
    int lux = (2500 / Vout - 500) / 10;
    //int lux=500/(10*((5-Vout)/Vout)); //Conversion to Lux
    return lux;
}

///////// Void //////////

void setup() {
    Serial.begin(115200);
    Serial.println("Ligth - Temperature - Humidity Sensors");

//Digital Temperature/Room Humidity//

    dht.begin(); //Initialize sensor
}

///////// Loop //////////

void loop() {

    delay(5000);

//Analog Photoresistor//

    int lux = int(Light2(analogRead(0)));
    int lux1 = int(Light2(analogRead(1)));
    int lux2 = int(Light2(analogRead(2)));
    int lux3 = int(Light2(analogRead(3)));
    int lux4 = int(Light2(analogRead(4)));

//Digital Temperature/Room Humidity//

    float h = dht.readHumidity(); // Read Humidity %

    float t = dht.readTemperature(); // Read temperature in Celsius
  
    // Check if sensor is reading or connected
    if (isnan(h) || isnan(t)) {
    Serial.println("Temperature - Humidity sensor disconnected");
    return;
    }

/*     ///////// Humidity
   Serial.print("Humidity:        "); 
   Serial.print(h);
   Serial.print(" %\t");
   Serial.println();

    ///////// Temperature Celsuis
    Serial.print("Temperature:     "); 
    Serial.print(t);
    Serial.print(" *C ");
    Serial.println();

    ///////////////////////////////////

    ///////// Lux 0

    Serial.print("L0:     "); 
    Serial.print(lux);
    Serial.print(" lux ");
    Serial.println();

    ///////// Lux 1

    Serial.print("L1:     "); 
    Serial.print(lux1);
    Serial.print(" lux ");
    Serial.println();

        ///////// Lux 2

    Serial.print("L2:     "); 
    Serial.print(lux2);
    Serial.print(" lux ");
    Serial.println();


        ///////// Lux 3

    Serial.print("L3:     "); 
    Serial.print(lux3);
    Serial.print(" lux ");
    Serial.println();

        ///////// Lux 1

    Serial.print("L4:     "); 
    Serial.print(lux4);
    Serial.print(" lux ");
    Serial.println();
 */




    while(Serial.available()) {
    message = Serial.readString();
    messageReady = true;
  }
  // Only process message if there's one
  if(messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(250); // ArduinoJson version 6+
    // Attempt to deserialize the message
    DeserializationError error = deserializeJson(doc,message);
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      Serial.println(message);
      messageReady = false;
      return;
    }
    if(doc["type"] == "request") {
      doc["type"] = "response";
      doc["S"] = serialDevice;
      // Get data from analog sensors
      doc["L0"] = lux;
      doc["L1"] = lux1; 
      doc["L2"] = lux2; 
      doc["L3"] = lux3; 
      doc["L4"] = lux4; 
      doc["T"] = t;
      doc["H"] = h;
      serializeJson(doc,Serial);
    }
    messageReady = false;
  }



}
