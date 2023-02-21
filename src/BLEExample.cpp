#include "BLEExample.hpp"
#include "MyLog.hpp"
#include "RgbLed.hpp"

namespace BLEExample {
    constexpr static auto UUID_characteristic = "19B10000-E8F2-537E-4F6C-D104768A1214";
    constexpr static auto UUID_service = "19B10000-E8F2-537E-4F6C-D104768A1215";
    enum class Command: uint8_t {
        LED_TOGGLE = 0
    };
    auto command_characteristic = BLETypedCharacteristic<Command>(UUID_characteristic, BLERead | BLEWrite);

    auto subscribedHandler(BLEDevice device, BLECharacteristic characteristic) -> void {
        MyLog::log("Event Characteristic::Subscribed");
    }
    auto unsubscribedHandler(BLEDevice device, BLECharacteristic characteristic) -> void {
        MyLog::log("Event Characteristic::Unsubscribed");
    }
    auto writeHandler(BLEDevice device, BLECharacteristic characteristic) -> void {
        MyLog::log("Event Characteristic::Write");
        if (characteristic.uuid() == command_characteristic.uuid()) {
            Command command;
            const auto ret = command_characteristic.readValue(*(uint8_t*)&command);
            if (ret != sizeof(Command)) {
                MyLog::log("Read wrong amount of bytes: ", ret);
            }
            if (command == Command::LED_TOGGLE) {
                MyLog::log("Got Command::LED_TOGGLE");
                toggleLED();
            } else {
                MyLog::log("Got unknown Command");
            }
        }
    }
    auto readHandler(BLEDevice device, BLECharacteristic characteristic) -> void {
        MyLog::log("Event Characteristic::Read");
    }

    auto connectedHandler(BLEDevice device) -> void {
        MyLog::log("Event Device::Connected");
    }
    auto disconnectedHandler(BLEDevice device) -> void {
        MyLog::log("Event Device::Disconnected");
    }
    auto discoveredHandler(BLEDevice device) -> void {
        MyLog::log("Event Device::Discovered");
    }
    auto setup() -> void {
        if (!BLE.begin()) {
            MyLog::log("starting BLE failed!");
            return;
        }
        MyLog::log("BLE started!");
        command_characteristic.setEventHandler(BLESubscribed,   subscribedHandler);
        command_characteristic.setEventHandler(BLEUnsubscribed, unsubscribedHandler);
        command_characteristic.setEventHandler(BLERead,         readHandler);
        command_characteristic.setEventHandler(BLEWritten,      writeHandler);



        BLE.setEventHandler(BLEConnected,        connectedHandler);
        BLE.setEventHandler(BLEDisconnected,     disconnectedHandler);
        BLE.setEventHandler(BLEDiscovered,       discoveredHandler);
        
        BLE.setLocalName("KnerzLocalName");
        BLE.setDeviceName("KnerzDeviceName");

        BLEService service(UUID_service);
        BLE.setAdvertisedService(service);

        service.addCharacteristic(command_characteristic);
        BLE.addService(service);


        // BLE.setPairable(Pairable::YES);
        BLE.setConnectable(true);
        BLE.setPairable(Pairable::NO);
        // BLE.setConnectable(false);

        BLE.advertise();
    }

    auto loop() -> void {
        BLE.poll(1000);
    }
}
