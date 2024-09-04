// Define the pins
const int irSensorPin = 2; // IR sensor output connected to digital pin 2

// Variables
unsigned long lastTime = 0;
unsigned long lastRPM = 0;
unsigned long currentTime = 0;
volatile unsigned long pulseCount = 0;

void setup() {
  pinMode(irSensorPin, INPUT_PULLUP); // Set IR sensor pin as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(irSensorPin), countPulses, RISING); // Attach interrupt to IR sensor pin
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  currentTime = millis();
  
  // Calculate RPM every 1000 milliseconds (1 second)
  if (currentTime - lastTime >= 1000) {
    unsigned long rpm = (pulseCount - lastRPM) * 60; // Calculate RPM
    lastRPM = pulseCount; // Update last RPM value
    lastTime = currentTime; // Update last time value
    Serial.println(rpm); // Print RPM to serial monitor
  }
}

void countPulses() {
  pulseCount++; // Increment pulse count when interrupt is triggered
}
