//////////////////////////////////////////////////////
////////////////// WSEEDSOLUTIONS ////////////////////
//////////////////////////////////////////////////////
////////////// Analog Photoresistor //////////////////
////////////////// Ligth sensor //////////////////////
//////////////////////////////////////////////////////
///////////// Temperature - Humidity /////////////////
//////////////////////////////////////////////////////
////////////// Arduino Sensor code ///////////////////
//////////////////////////////////////////////////////


#include <ArduinoJson.h>

#include <AverageValue.h>

// Number of values to calculate with. Prevents memory problems
const long MAX_VALUES_NUM = 10;

AverageValue<long> aveL0(MAX_VALUES_NUM);
AverageValue<long> aveL1(MAX_VALUES_NUM);
AverageValue<long> aveT(MAX_VALUES_NUM);
AverageValue<long> aveH(MAX_VALUES_NUM);




//comunication
String message = "";
bool messageReady = false;

////////////// Analog Photoresistor //////////////////

//const int ledPin = 9; //LED Digital output D9 pin
int val = 0; //Initial value ligth 

//Variables for LED on/off
int lightVal;

int voltageread = 0;  //Voltage reading for lux conversion

//////////////// Code for DHT 11 sensor //////////////

#include "DHT.h"
#define DHTPIN 2     // Analog pin 2 
#define DHTTYPE DHT11   // DHT 11 sensor

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino


//Method for voltage to lux conversion//
double Light (int voltageread)
{
    double Vout = voltageread * 0.0048828125; // Voltage output
    //int lux = (2500 / Vout - 500) / 10;
     int lux=500/(10*((5-Vout)/Vout)); //Conversion to Lux
    return lux;
}


///////// Void //////////

void setup() {
    Serial.begin(9600);
    Serial.println("Ligth - Temperature - Humidity Sensors");

//Analog Photoresistor//

    //pinMode(ledPin, OUTPUT); //LED pin 9 output



//Digital Temperature/Room Humidity//

    dht.begin(); //Initialize sensor
}

///////// Loop //////////

void loop() {
    //delay general para el arduino, debe estar presente en el wifi module
    delay(1000);

//Analog Photoresistor//

    int lux0 = int(Light(analogRead(0)));
    aveL0.push(lux0);
    int lux1 = int(Light(analogRead(1)));
    aveL1.push(lux1);



//Digital Temperature/Room Humidity//

    float h = dht.readHumidity(); // Read Humidity %
    aveH.push(h);

    float t = dht.readTemperature(); // Read temperature in Celsius
    aveT.push(t);
  
    // Check if sensor is reading or connected
    if (isnan(h) || isnan(t)) {
    Serial.println("Temperature - Humidity sensor disconnected");
    return;
    }

    while(Serial.available()) {
    message = Serial.readString();
    messageReady = true;
  }
  // Only process message if there's one
  if(messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(1024); // ArduinoJson version 6+
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
      // Get data from analog sensors
      doc["L0"] = aveL0.average();
      doc["L1"] = aveL1.average(); 
      doc["T"] = aveT.average();
      doc["H"] = aveH.average();
      serializeJson(doc,Serial);
    }
    messageReady = false;
  }



}