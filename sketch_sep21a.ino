#include "EmonLib.h"             // Include Emon Library

#define VOLT_CAL 592
const int analogPin = A4;     // Analog pin connected to ACS712 output
const float Vref = 5.0;       // Arduino reference voltage (5V for most cases)
const float sensitivity = 0.185; 
EnergyMonitor emon1;             // Create an instance

void setup()
{  
  Serial.begin(9600);
  
  emon1.voltage(1, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift
}

void loop()
{
  emon1.calcVI(25, 1000);         // Calculate all. No.of half wavelengths (crossings), time-out
  
  float supplyVoltage = emon1.Vrms;             // Extract Vrms into Variable
  if (supplyVoltage >= 100)
  {
    // Print the voltage reading in CSV format
    Serial.print(millis());  // Print timestamp
    Serial.print(",");
    Serial.print("Voltage = ");
    Serial.println(supplyVoltage); // Print voltage
  }
  else
  {
    // Print "Power OFF" in CSV format
    Serial.print(millis());  // Print timestamp
    Serial.print(",");
    Serial.println(0); // Print 0 to represent power off
  }
  
  delay(1000); // Delay to control the rate of serial output
   int sensorValue = analogRead(analogPin);  // Read the analog value
  float voltage = (sensorValue / 1023.0) * Vref;  // Convert to voltage
  
  // Calculate current in Amperes
  float current = (voltage - (Vref / 2)) / sensitivity;
  
  // Send data in a format suitable for serial plotter
   Serial.print("Sensor value = ");
  Serial.print(sensorValue);
  Serial.print(",");
   Serial.print("Current value = ");
  Serial.println(current);
  // Delay for a second
}