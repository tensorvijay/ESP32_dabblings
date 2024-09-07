#include <SPI.h>

#define SENSITIVITY 500.0f

// ZMPT101B sensor output connected to analog pin A0
// and the voltage source frequency is 50 Hz.
int voltageSensorPin = 34;

// Define the pins
const int irSensorPin = 2; // IR sensor output connected to digital pin 2

// Variables
unsigned long lastTime = 0;
unsigned long lastRPM = 0;
unsigned long currentTime = 0;
volatile unsigned long pulseCount = 0;

// Define MAX6675 pins for the default SPI interface
int thermoCS = 15; // Connect MAX6675 CS to GPIO 15

// MAX6675 setup
SPIClass mySPI(VSPI); // Use VSPI for ESP32, select the appropriate SPI bus
const int thermoCLK = 18; // Connect MAX6675 SCK to GPIO 18
const int thermoDO = 19; // Connect MAX6675 MISO to GPIO 19

void IRAM_ATTR countPulses() {
  pulseCount++; // Increment the pulse count
}

void setup() {
  Serial.begin(9600);

  pinMode(voltageSensorPin, INPUT); // Set ZMPT101B sensor pin as input
  pinMode(irSensorPin, INPUT_PULLUP); // Set IR sensor pin as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(irSensorPin), countPulses, RISING); // Attach interrupt to IR sensor pin

  mySPI.begin(thermoCLK, thermoDO, thermoCS); // Initialize VSPI for MAX6675

  // Initialize the MAX6675 library
  mySPI.setBitOrder(MSBFIRST);
  mySPI.setDataMode(SPI_MODE1);
}

void loop() {
  // Read temperature
  double celsius = readMAX6675();

  // Print results
  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.println(" °C");

  // Read voltage from ZMPT101B sensor (assuming it's a simple analog voltage)
  int rawVoltage = analogRead(voltageSensorPin);
  float voltage = (rawVoltage / 4095.0) * 3.3; // Assuming 12-bit ADC, 3.3V reference
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  currentTime = millis();

  // Calculate RPM every 1000 milliseconds (1 second)
  if (currentTime - lastTime >= 1000) {
    unsigned long rpm = (pulseCount - lastRPM) * 60; // Calculate RPM
    lastRPM = pulseCount; // Update last RPM value
    lastTime = currentTime; // Update last time value
    Serial.print("RPM: ");
    Serial.println(rpm); // Print RPM to the serial monitor
  }

  delay(1000); // Delay for 1 second
}

double readMAX6675() {
  mySPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1)); // Configure SPI settings for MAX6675
  digitalWrite(thermoCS, LOW); // Select the MAX6675

  // Read data from MAX6675
  int value;
  value = mySPI.transfer(0);
  value <<= 8;
  value |= mySPI.transfer(0);
  digitalWrite(thermoCS, HIGH); // Deselect the MAX6675
  mySPI.endTransaction();

  if (value & 0x4) {
    return NAN; // There was an error reading the temperature
  } else {
    int celsius = (value >> 3) * 0.25;
    return celsius;
  }
}



