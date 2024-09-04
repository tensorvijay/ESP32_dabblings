#include <SPI.h>
#include "max6675.h"
#include <ZMPT101B.h>

#define SENSITIVITY 500.0f

// ZMPT101B sensor output connected to analog pin A0
// and the voltage source frequency is 50 Hz.
ZMPT101B voltageSensor(A0, 50.0);

// Define the pins
const int irSensorPin = 2; // IR sensor output connected to digital pin 2

// Variables
unsigned long lastTime = 0;
unsigned long lastRPM = 0;
unsigned long currentTime = 0;
volatile unsigned long pulseCount = 0;



int thermoCLK = 13;
int thermoCS = 12;
int thermoDO = 11;

int voltageSensorPin = A0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  Serial.begin(9600);

  voltageSensor.setSensitivity(SENSITIVITY);

  pinMode(irSensorPin, INPUT_PULLUP); // Set IR sensor pin as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(irSensorPin), countPulses, RISING); // Attach interrupt to IR sensor pin
}

void loop() {
  // Read temperature
  double celsius = thermocouple.readCelsius();
  
  // Print results
  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.println(" °C");
  
  float voltage = voltageSensor.getRmsVoltage();
  Serial.println(voltage);

  currentTime = millis();
  
  // Calculate RPM every 1000 milliseconds (1 second)
  if (currentTime - lastTime >= 1000) {
    unsigned long rpm = (pulseCount - lastRPM) * 60; // Calculate RPM
    lastRPM = pulseCount; // Update last RPM value
    lastTime = currentTime; // Update last time value
    Serial.println("RPM: ");
    Serial.println(rpm); // Print RPM to serial monitor
  }

  delay(1000); // Delay for 1 second
}
