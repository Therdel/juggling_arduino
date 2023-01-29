//
// Arduino main stub file. Calls into Rust.
//

#include <memory>
#include <Arduino.h>
#include <Arduino_LSM9DS1.h>

#include "RgbLed.hpp"
#include "BLEExample.hpp"

//
// FFI C -> Rust
//
extern "C" {
    void arduino_setup();
    void arduino_loop();
}

rtos::Thread g_thread;
int g_highCount = 0;
rtos::Mutex g_mutex;

extern "C" void setup() {
    arduino_setup();
    Serial.begin(9600);
    while (!Serial) _NOP();
    Serial.println("Started");

    pinMode(LED_BUILTIN, OUTPUT);

    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (true);
    }
    while(Serial.read() != 's') {
        rtos::ThisThread::sleep_for(std::chrono::seconds(1));
        Serial.println("Waiting for 's'...");
    }
    // ble_example();

    g_thread.start([] {
        while(true) {
            rtos::ScopedMutexLock lock(g_mutex);
            static auto lastCount = 0;
            if(g_highCount > lastCount) {
                lastCount = g_highCount;
                Serial.print("High: ");
                Serial.print(g_highCount);
                Serial.println(" times");
            }
        }
    });
}

auto toggleLED() {
    static bool lastBlink = false;
    if (lastBlink) {
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    lastBlink = !lastBlink;
}

extern "C" void rampLED() {
    static uint8_t lastBrightness = 0;
    static RgbLed led;
    led.setOnNess(lastBrightness--);
}

auto printAccelerationString() {
    if (IMU.accelerationAvailable()) {
        float x, y, z;
        if (!IMU.readAcceleration(x, y, z)) {
            Serial.println("Couldn't read acceleration data (although claimed to be available)");
        } else {
            Serial.print(x);
            Serial.print(" | ");
            Serial.print(y);
            Serial.print(" | ");
            Serial.println(z);
        }
    }
}

enum class AccelerationState {
    NoChange,
    EdgeHigh,
    EdgeLow
};

auto accelerationChange(float accel) -> AccelerationState {
    static bool lastWasHigh = false;
    static constexpr float high_thresh = 4.0f;
    static constexpr float low_thresh = 1.0f;

    auto state = AccelerationState::NoChange;
    if (lastWasHigh) {
        if (accel <= low_thresh) {
            state = AccelerationState::EdgeLow;
            lastWasHigh = false;
        }
    }
    else {
        if (accel >= high_thresh) {
            state = AccelerationState::EdgeHigh;
            lastWasHigh = true;
        }
    }
    return state;
}

auto printAccelerationState(AccelerationState state)
{
    if (state != AccelerationState::NoChange) {
        const char *new_state;
        if (state == AccelerationState::EdgeHigh) {
            new_state = "HIGH";
        } else {
            new_state = "LOW";
        }
        const auto message = std::string("Accel turned ") + new_state;
        Serial.println(message.c_str());
    }
}

extern "C" void loop()
{
    arduino_loop();
    // printAccelerationString();

    if (IMU.accelerationAvailable())
    {
        float x, y, z;
        IMU.readAcceleration(x, y, z);
        float acceleration = static_cast<float>(std::sqrt(sq(x) + sq(y) + sq(z)));

        // Serial.printf("Accel: % .3f\n", acceleration);
        auto state = accelerationChange(acceleration);
        if (state == AccelerationState::EdgeHigh) {
            rtos::ScopedMutexLock lock(g_mutex);
            ++g_highCount;

            toggleLED();
        }
        // printAccelerationState(state);
    }
}
