#include <Arduino.h>
#include "RgbLed.hpp"

RgbLed::RgbLed() {
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
}

auto RgbLed::setColor(uint8_t r, uint8_t g, uint8_t b) -> void {
    setR(r); setG(g); setB(b);
}

auto RgbLed::setOnNess(uint8_t brightness) -> void {
    setColor(brightness, brightness, brightness);
}

auto RgbLed::getR() -> uint8_t { return analogRead(LEDR); }
auto RgbLed::getG() -> uint8_t { return analogRead(LEDG); }
auto RgbLed::getB() -> uint8_t { return analogRead(LEDB); }

auto RgbLed::setR(uint8_t value) -> void { analogWrite(LEDR, value); }
auto RgbLed::setG(uint8_t value) -> void { analogWrite(LEDG, value); }
auto RgbLed::setB(uint8_t value) -> void { analogWrite(LEDB, value); }


auto toggleLED() -> void {
    static bool lastBlink = false;
    if (lastBlink) {
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    lastBlink = !lastBlink;
}

extern "C" auto rampLED() -> void {
    static uint8_t lastBrightness = 0;
    static RgbLed led;
    led.setOnNess(lastBrightness--);
}