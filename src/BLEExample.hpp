#pragma once
#include <ArduinoBLE.h>

auto read_handler(BLEDevice device, BLECharacteristic characteristic) -> void {
    Serial.println("Event: Read");
}

auto subscribed_handler(BLEDevice device, BLECharacteristic characteristic) {
    Serial.println("Event: Subscribed");
}

auto ble_example() {
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        return;
    }
    Serial.println("BLE started!");
    BLE.setLocalName("KnerzLocalName");
    BLE.setDeviceName("KnerzDeviceName");
/*
  BLEConnected = 0,
  BLEDisconnected = 1,
  BLEDiscovered = 2,
*/
    BLE.setEventHandler(BLEConnected, [](BLEDevice central) {Serial.println("Central Connected");});
    BLE.setEventHandler(BLEDisconnected, [](BLEDevice central) {Serial.println("Central Disconnected");});
    BLE.setEventHandler(BLEDiscovered, [](BLEDevice central) {Serial.println("Central Discovered");});

    BLECharacteristic message("1337", BLERead | BLEWrite, "TestKnerz");
    message.setEventHandler(BLESubscribed,   [](auto, auto) { Serial.println("Event: Subscribed"); });
    message.setEventHandler(BLEUnsubscribed, [](auto, auto) { Serial.println("Event: Unsubscribed"); });
    message.setEventHandler(BLERead,         [](auto, auto) { Serial.println("Event: Read"); });
    message.setEventHandler(BLEWritten,      [](auto, auto) { Serial.println("Event: Written"); });

    BLEService service;
    service.addCharacteristic(message);
    BLE.addService(service);


    BLE.advertise();
    BLE.setConnectable(true);
    while(true) {};
}