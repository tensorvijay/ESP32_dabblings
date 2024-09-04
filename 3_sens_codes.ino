#include "max6675.h"

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

#include "EmonLib.h"             // Include Emon Library
#define VOLT_CAL 592
const int analogPin1 = A0; 
const float Vref = 5.0;       // Arduino reference voltage (5V for most cases)
const float sensitivity = 0.185; 
EnergyMonitor emon1;             // Create an instance
const int sensorPin = 2;  // Pin connected to the OUT of the HW-201 sensor
unsigned long startTime;
volatile unsigned long elapsedTime;
volatile unsigned long revCount;
void countRevolutions() 
{
  revCount++;
}
   

void setup()
{  
  Serial.begin(9600);


  // wait for MAX chip to stabilize
  delay(500);

  emon1.voltage(1, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift
    pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), countRevolutions, FALLING);
  revCount = 0;
  startTime = millis();
}

void loop()
{  // basic readout test, just print the current temp
  
  //Serial.print("C = "); 
  Serial.print(",");
  Serial.print(thermocouple.readCelsius());
  
 
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
  emon1.calcVI(25, 1000);         // Calculate all. No.of half wavelengths (crossings), time-out
  
  float supplyVoltage = emon1.Vrms;             // Extract Vrms into Variable
  if (supplyVoltage >= 100)
  {
    // Print the voltage reading in CSV format
    
    Serial.print(",");
    //Serial.print("Voltage = ");
    Serial.print(supplyVoltage); // Print voltage
  }
  else
  {
    
    Serial.print(",");
    Serial.print(0); // Print 0 to represent power off
  }
  
  delay(1000); // Delay to control the rate of serial output

   



 // Calculate RPM and print to Serial Plotter every second
  if (millis() - startTime >= 1000) {
    elapsedTime = millis() - startTime;
    float rpm = (revCount / (float(elapsedTime) / 1000.0)) * 60.0;
    //Serial.print("rpm=");
     Serial.print(",");
    Serial.print(rpm);
     Serial.println(",");
   
    
    // Reset variables
    revCount = 0;
    startTime = millis();
  }
}