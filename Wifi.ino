#include <WiFi.h>

const char* ssid = "vijay";          // Your WiFi SSID
const char* password = "vijay@123";  // Your WiFi Password


WiFiServer server(81); // Port 80

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read the Hall sensor value
  int hallValue = hallRead();

  Serial.print("Hall Sensor Value: ");
  Serial.println(hallValue);

  // Your code here
  delay(1000); // You can adjust the delay as needed
}

