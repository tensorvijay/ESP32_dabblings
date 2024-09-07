#include "max6675.h"
#include "BluetoothSerial.h"

int thermoDO = 4;  //so
int thermoCS = 5; //cs
int thermoCLK = 2; //sck
int voltage1  = 32;   //voltage input
int voltage2  = 33;   //volrage input from sensor
const int sensorPin = 18;  // Define the pin where your tachometer sensor is connected vout HW-201
volatile unsigned long pulseCount = 0;
unsigned long lastTime = 0;
unsigned long currentTime = 0;

BluetoothSerial SerialBT;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  Serial.begin(9600);
  SerialBT.begin("All_four_senors");
  Serial.println("All_four_senors");
  pinMode(voltage1, INPUT);
  pinMode(voltage2, INPUT);
  pinMode(sensorPin, INPUT_PULLUP);  // Set the sensor pin as an input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulses, RISING); // Attach interrupt to the sensor pin


  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
   currentTime = millis();
   int rawVoltage1 = analogRead(voltage1);
   int rawVoltage2 = analogRead(voltage2);
   float voltage1 = (rawVoltage1 / 4095.0) * 3.3;
   float voltage2 = (rawVoltage2 / 4095.0) * 3.3;
   Serial.print("Voltage1: ");  //voltage input
   Serial.print(voltage1);
   Serial.println(" V");
   Serial.print("Voltage2: ");  //voltage input
   Serial.print(voltage2);
   Serial.println(" V");
   SerialBT.print("Voltage1: ");  //voltage input
   SerialBT.print(voltage1);
   SerialBT.println(" V");
   SerialBT.print("Voltage2: ");  //voltage input
   SerialBT.print(voltage2);
   SerialBT.println(" V");
  // Calculate RPM every 1000 milliseconds (1 second)
  if (currentTime - lastTime >= 1000) {
    unsigned long rpm = (pulseCount * 60) / 2; // Adjust for the number of pulses per rotation (e.g., 2 pulses per rotation)
    lastTime = currentTime;
    Serial.print("RPM: ");
    Serial.println(rpm);
    SerialBT.print("RPM: ");
    SerialBT.println(rpm);
    pulseCount = 0;
  }
   SerialBT.print("C = "); 
   SerialBT.println(thermocouple.readCelsius());
   //Serial.print("F = ");
   //Serial.println(thermocouple.readFahrenheit());
 
   // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
   delay(1000);
}

void countPulses() {
  pulseCount++; // Increment the pulse count
}
