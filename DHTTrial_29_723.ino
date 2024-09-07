#include "DHT.h"
#define DHT11PIN 16
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define LED 2
AsyncWebServer server(80);

const char* ssid = "kousalya_5G";          // Your WiFi SSID
const char* password = "planck#6.2";  // Your WiFi Password





DHT dht(DHT11PIN, DHT11);

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "ON"){
    digitalWrite(LED, HIGH);
  }
  if (d=="OFF"){
    digitalWrite(LED, LOW);
  }
}



void setup()
{
  
  //Serial.begin(115200);
/* Start the DHT11 Sensor */
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  dht.begin();
  
}

void loop()
{
  WebSerial.println("Hello!");
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  WebSerial.println("Temperature: ");
  WebSerial.println(temp);
  WebSerial.println("ºC ");
  WebSerial.println("Humidity: ");
  WebSerial.println(humi);
  delay(1000);
}