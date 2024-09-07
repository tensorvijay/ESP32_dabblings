#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer;
BLECharacteristic* pCharacteristic;

int hallValue = 0;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
    }

    void onDisconnect(BLEServer* pServer) {
    }
};

void setup() {
    Serial.begin(115200);

    BLEDevice::init("HallEffectSensor");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks);

    BLEService *pService = pServer->createService(BLEUUID("0000FFF0-0000-1000-8000-00805F9B34FB"));
    pCharacteristic = pService->createCharacteristic(
        BLEUUID("0000FFF1-0000-1000-8000-00805F9B34FB"),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->start();

    hallValue = hallRead();
}

void loop() {
    int newHallValue = hallRead();

    if (newHallValue != hallValue) {
        hallValue = newHallValue;
        pCharacteristic->setValue(hallValue);
        pCharacteristic->notify();
        Serial.println("Hall Value: " + String(hallValue));
    }

    delay(1000);
}

int hallRead() {
    int sensorValue = hallRead(); // Read the Hall effect sensor
    return sensorValue;
}
