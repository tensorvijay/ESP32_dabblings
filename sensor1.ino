const int sensorPin = 18;  // Define the pin where your tachometer sensor is connected
volatile unsigned long pulseCount = 0;
unsigned long lastTime = 0;
unsigned long currentTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(sensorPin, INPUT_PULLUP);  // Set the sensor pin as an input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulses, RISING); // Attach interrupt to the sensor pin
}

void loop() {
  currentTime = millis();

  // Calculate RPM every 1000 milliseconds (1 second)
  if (currentTime - lastTime >= 1000) {
    unsigned long rpm = (pulseCount * 60) / 2; // Adjust for the number of pulses per rotation (e.g., 2 pulses per rotation)
    lastTime = currentTime;
    Serial.print("RPM: ");
    Serial.println(rpm);
    pulseCount = 0;
  }
}

void countPulses() {
  pulseCount++; // Increment the pulse count
}
