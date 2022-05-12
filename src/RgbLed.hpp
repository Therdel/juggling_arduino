#pragma once

class RgbLed {
public:
  RgbLed();

  auto setColor(uint8_t r, uint8_t g, uint8_t b) -> void;

  auto setOnNess(uint8_t brightness) -> void;

  auto getR() -> uint8_t;
  auto getB() -> uint8_t;
  auto getG() -> uint8_t;

  auto setR(uint8_t value) -> void;
  auto setG(uint8_t value) -> void;
  auto setB(uint8_t value) -> void;
};