#pragma once

#include <Arduino.h>

struct LampState
{
  bool isOn = true;

  uint8_t brightness = 100;
  uint8_t speed = 128;
  uint8_t scale = 128;

  uint8_t currentEffect = 0;
};