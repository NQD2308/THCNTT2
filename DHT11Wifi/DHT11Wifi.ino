#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#define DHTTYPE DHT11  
#include "DHT.h"
#define DHTPIN D6 
DHT dht(DHTPIN, DHTTYPE);

//OUTPUT Devices
int led = D5;
int bell = D7;

// MQ-2 Sensor
int sensor = A0;
int gas_value;


const char *SSID = "HSU_Students";
const char *PASSWORD = "dhhs12cnvch";
const char *URL = "http://10.106.22.150:1234/create"; 

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;

void setup() {
  pinMode(led,OUTPUT);
  pinMode(bell,OUTPUT);
  pinMode(sensor,INPUT);

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWORD);
  // wait for WiFi connection
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void postJsonData() {

  Serial.print("connecting to ");
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, URL)) {  // HTTP
      Serial.print("[HTTP] POST...\n");
      //gui du lieu len server dang JSON
      const int capacity = JSON_OBJECT_SIZE(5);
      StaticJsonDocument<capacity> doc;
      dht.begin();

      digitalWrite(led, LOW);
      digitalWrite(bell, LOW);

      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      
      float analogValue =analogRead(sensor);
      int g = map(analogValue, 0, 1024, 0, 100);
      if(g >= 40)
      {
        digitalWrite(led, HIGH);
        digitalWrite(bell, HIGH);
        delay(100);
        digitalWrite(bell, LOW);
        delay(100);
      }

      delay(1000);

      if(g >= 40)
      {
        digitalWrite(bell, HIGH);
        delay(100);
        digitalWrite(bell, LOW);
        delay(100);
      }


      doc["id"] = "1";
      doc["Temp"] = t;
      doc["Hud"] = h;
      doc["Gas"] = g;

      
      char output[2048];
      serializeJson(doc, Serial);
      serializeJson(doc, output);

     
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(output);
      Serial.println("done");
      Serial.println(httpCode); //Print HTTP return code
      // file found at server
//       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
//           String payload = http.getString();
//           Serial.println(payload);
// //          char input[50];
// //          payload.toCharArray(input, 50);
// //          //parseJson
// //          Serial.println("Begin parse json data ...");
// //          DynamicJsonDocument docResponse(2048);
// //          DeserializationError err = deserializeJson(docResponse, payload);
// //          if (err) {
// //            Serial.print(F("deserializeJson() failed with code "));
// //            Serial.println(err.c_str());
// //          }
//           //auto name = docResponse["name"].as<char*>();
//           Serial.println("done");
        
//       } else {
//         Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
//       }

      http.end(); //Close connection Serial.println();
      Serial.println("closing connection");
    }
  }
}
void loop() {
  postJsonData();
  delay(1000);
}