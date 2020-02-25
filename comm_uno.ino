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

//comunication
String message = "";
bool messageReady = false;

////////////// Analog Photoresistor //////////////////

int sensorPin = A0; //Sensor connected to Analog 0
//const int ledPin = 9; //LED Digital output D9 pin
int val = 0; //Initial value ligth 

int lightCal; //Variables for LED on/off
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
    lightCal = analogRead(sensorPin);

//Digital Temperature/Room Humidity//

    dht.begin(); //Initialize sensor
}

///////// Loop //////////

void loop() {

    delay(3*60*1000);

//Analog Photoresistor//

    int lux = int(Light(analogRead(0)));

    //Serial.print("Light Intensity: ");
    //Serial.print(lux); //Print Lux value 
    //Serial.println(" Lux");
    
    //lightVal = analogRead(sensorPin);
    
    //if (lightVal < lightCal - 50)
    //{
    //    digitalWrite(9, HIGH); //If ligth intensity is low turn LED on
    //}

    //else
    //{
    //    digitalWrite(9, LOW); //Else turn LED off
    //}

//Digital Temperature/Room Humidity//

    float h = dht.readHumidity(); // Read Humidity %

    float t = dht.readTemperature(); // Read temperature in Celsius

  
    // Check if sensor is reading or connected
    if (isnan(h) || isnan(t)) {
    Serial.println("Temperature - Humidity sensor disconnected");
    return;
    }

    ///////// Humidity
   // Serial.print("Humidity:        "); 
   //Serial.print(h);
   // Serial.print(" %\t");
    //  Serial.println();

    ///////// Temperature Celsuis
    //Serial.print("Temperature:     "); 
    //Serial.print(t);
    //Serial.print(" *C ");
    //Serial.println();

    ///////////////////////////////////




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
      Serial.println(message)
      messageReady = false;
      return;
    }
    if(doc["type"] == "request") {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["L"] = lux; 
      doc["T"] = t;
      doc["H"] = h;
      serializeJson(doc,Serial);
    }
    messageReady = false;
  }



}
