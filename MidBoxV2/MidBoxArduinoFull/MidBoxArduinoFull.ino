//////////////////////////////////////////////////////
////////////////// WSEEDSOLUTIONS ////////////////////
//////////////////////////////////////////////////////
///////////////////// TLS2561 ////////////////////////
//////////////////////////////////////////////////////
////////////////////  BMP280 /////////////////////////
/////////////////////  (2)  //////////////////////////
//////////////////////////////////////////////////////
///////////// Temperature - Humidity /////////////////
//////////////////////////////////////////////////////
////////////// Arduino Sensor code ///////////////////
//////////////////////////////////////////////////////

/// imports 

#include <ArduinoJson.h>

///// TLS2561 && BME280

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_TSL2561_U.h>

/**************************************************************************/

#define SEALEVELPRESSURE_HPA (1023.25)

/**************************************************************************/
Adafruit_BME280 bme;

// assignations

Adafruit_TSL2561_Unified tsl_1 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

Adafruit_TSL2561_Unified tsl_2 = Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH, 12345);


//////////////////////
////Serial device/////
//////////////////////

String serialDevice = "w_1";

//////////////////////

//comunication
String message = "";
bool messageReady = false;


/**************************************************************************/
void configureSensor1(void)
{

  tsl_1.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */

  
  tsl_1.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
    

}

void configureSensor2(void)
{
  
  tsl_2.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  
  tsl_2.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
    
  
}

/**************************************************************************/

///////// Void //////////

void setup() {
    Serial.begin(115200);
    Serial.println("Ligth - Temperature - Humidity Sensors - Presion - Altitud");
  /* Initialise the sensor */
  /* Initialise the TSL2561 sensor */
 
  if(!tsl_1.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561_1 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

if(!tsl_2.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561_2 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  
  
  /* Setup the TSL2561 sensor gain and integration time */
  configureSensor1();

  configureSensor2();
  
  /* We're ready to go! */
  Serial.println("");

}

///////// Loop //////////

void loop() {

    delay(5000);






//Luz //
  sensors_event_t event1;
  tsl_1.getEvent(&event1);

    sensors_event_t event2;
  tsl_2.getEvent(&event2);
  /* Display the results (light is measured in lux) */
  float lux = -1.0;
    float lux1 = -1.0;
  if (event1.light)
  {
  lux = event1.light; 
  }


  if (event2.light)
  {
   lux1  = event2.light; 
  }



//Humedad//

    float h = float(bme.readHumidity()); // Read Humidity %


    // temperatura

float t = float(bme.readTemperature()); // Read temperature in Celsius
    // altitud 

float a = float(bme.readAltitude(SEALEVELPRESSURE_HPA));
    // presion
float p = float(bme.readPressure() / 100.0F);
    
  
    // Check if sensor is reading or connected
    if (isnan(lux) ) {
    Serial.println("Lux");
    return;
    }
    if (isnan(lux1) ) {
    Serial.println("Temperature - Humidity sensor disconnected");
    return;
    }
        if (isnan(h) ) {
    Serial.println("Humidity ");
    return;
    }
        if ( isnan(t)) {
    Serial.println("Temperature ");
    return;
    }
        if (isnan(p) ) {
    Serial.println("presiion");
    return;
    }
        if (isnan(a) ) {
    Serial.println("Altitud");
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
      doc["S"] = serialDevice;
      // Get data from analog sensors
      doc["L0"] = lux;
      doc["L1"] = lux1; 
            doc["T"] = t;
      doc["H"] = h;
      doc["A"] = a; 
      doc["P"] = p; 

      serializeJson(doc,Serial);
    }
    messageReady = false;
  }



}
