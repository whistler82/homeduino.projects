#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Homey.h>
#include <SimpleDHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port D5 on the Arduino
#define ONE_WIRE_BUS D5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


unsigned long previousMillis = 0;
const unsigned long interval = 10000; //Interval in milliseconds

void wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin("SSID", "SSIDPASS");
    uint8_t timeout = 30;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (timeout<1) break;
    }
    if (WiFi.status() == WL_CONNECTED) {
      //Print IP address
      Serial.print("Connected to WiFi! (");
      Serial.print(WiFi.localIP());
      Serial.println(")");
    }
  }
}

void setup() {
  Serial.begin(115200);
  Homey.begin("DS18B20 sensor test");
  Homey.addCapability("measure_temperature");
}

void loop() {
  wifi();
  Homey.loop();
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    updateSensor();
  }
}

void updateSensor() {
    // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print((float)tempC); Serial.print(" *C, ");
    Homey.trigger("temperature", (float) tempC);
    Homey.setCapabilityValue("measure_temperature", (float) tempC);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
}
  //byte temperature = 0;
  //byte humidity = 0;
  //int err = SimpleDHTErrSuccess;
  //if ((err = dht11.read(PIN_DHT, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DS18B20 failed, err="); Serial.println(err);
    //return;
  //}

  //Serial.print((int)temperature); Serial.print(" *C, ");

  //Homey.trigger("temperature", (int) temperature);
  //Homey.setCapabilityValue("measure_temperature", (int) temperature)
  
//}
