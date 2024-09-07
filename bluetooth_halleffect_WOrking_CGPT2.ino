#include "BluetoothSerial.h"
#include "esp_adc_cal.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

// Define ADC constants for Hall sensor
#define DEFAULT_VREF 1100
#define NO_OF_SAMPLES 64
#define HALL_SENSOR_PIN 36  // The Hall sensor is typically connected to GPIO 36

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_HallSensor"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with Bluetooth!");
  // Configure ADC for Hall sensor
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
}

void loop() {
  // Read Hall sensor value
  int hallValue = 0;
  for (int i = 0; i < NO_OF_SAMPLES; i++) {
    hallValue += adc1_get_raw(ADC1_CHANNEL_0);
  }
  hallValue /= NO_OF_SAMPLES;

  // Print and send Hall sensor value over Bluetooth
  Serial.println("Hall Sensor Value: " + String(hallValue));
  SerialBT.println("Hall Sensor Value: " + String(hallValue));

  delay(1000); // Adjust the delay as needed
}
