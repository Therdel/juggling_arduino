#include <memory>
#include <Arduino.h>
#include <Arduino_LSM9DS1.h>

#include "RgbLed.hpp"
#include "BLEExample.hpp"

rtos::Mutex g_mutex;
bool g_wokeUp = false;
rtos::ConditionVariable g_cond(g_mutex);
rtos::Thread g_thread;

auto g_highCount = 0;

void setup() {
  
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Started");

  pinMode(LED_BUILTIN, OUTPUT);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  while(Serial.read() != 's') { 
    delay(1000);
    Serial.println("Waiting for 's'...");
  }
  ble_example();
  /*
  for(int i=0;i<100;++i)
    Serial.println();

  g_thread.start([] {
    Serial.println("Thread started!");

    rtos::ThisThread::sleep_for(2000);
    Serial.println("Thread woke up!");
    {
      g_mutex.lock();
      g_wokeUp = true;
      g_cond.notify_all();

      g_mutex.unlock(); 
    }

    while(true) {
      g_mutex.lock();
      static auto lastCount = 0;
      if(g_highCount > lastCount) {
        lastCount = g_highCount;
        Serial.printf("High: %2d times\n", g_highCount);
      }

      g_mutex.unlock(); 
    }
  });
  */
}

auto toggleLED() {
  static bool lastBlink = false;
  if(lastBlink) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  lastBlink = !lastBlink;
}

auto printAccelerationString() {
  if(IMU.accelerationAvailable()) {
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
  static constexpr float high_thresh = 3.0f;
  static constexpr float low_thresh = 1.0f;

  auto state = AccelerationState::NoChange;
  if(lastWasHigh) {
    if(accel <= low_thresh) {
      state = AccelerationState::EdgeLow;
      lastWasHigh = false;
    }
  } else {
    if(accel >= high_thresh) {
      state = AccelerationState::EdgeHigh;
      lastWasHigh = true;
    }
  }
  return state;
}

auto printAccelerationState(AccelerationState state) {
  if(state != AccelerationState::NoChange) {
    const char *new_state;
    if(state == AccelerationState::EdgeHigh) {
      new_state = "HIGH";
    } else {
      new_state = "LOW";
    }
    const auto message = std::string("Accel turned ") + new_state;
    Serial.println(message.c_str());
  }
}

void loop() {
  // printAccelerationString();
  {
    g_mutex.lock();
    if(not g_wokeUp) {
      g_cond.wait_for(2);   
      if(g_wokeUp) {
        Serial.println("MainThread heard a wake up!");
      }
    }
    g_mutex.unlock();
  }

  //delay(15);
  if(IMU.accelerationAvailable()) {
    float x, y, z;
    IMU.readAcceleration(x, y, z);
    float acceleration = static_cast<float>(std::sqrt(sq(x) + sq(y) + sq(z)));

    // Serial.printf("Accel: % .3f\n", acceleration);
    auto state = accelerationChange(acceleration);
    if(state == AccelerationState::EdgeHigh) {
      g_mutex.lock();
      ++g_highCount;
      g_mutex.unlock();

      toggleLED();
    }
    //printAccelerationState(state);
  }
}

