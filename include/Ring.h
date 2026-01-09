#pragma once

#include <FastLED.h>

#include "LedConfiguration.h"
#include "LampState.h"

#include "DEBUG.h"

const uint8_t SOLID_PATTERN[16] PROGMEM = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
const uint8_t DASHED_PATTERN[16] PROGMEM = {255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0};
const uint8_t LOAD_PATTERN[16] PROGMEM = {0, 0, 42, 84, 127, 212, 255, 0, 0, 42, 84, 127, 212, 255};

const uint16_t DURATION = 1000; // ms

enum class RingType : uint8_t
{
  NONE = 0,
  SOLID,
  DASHED,
  LOAD
};

class Ring
{
public:
  Ring(LedConfiguration &ledConf) : ledConfig(ledConf) {}

  bool view()
  {
    if (type == RingType::NONE)
      return false;

    switch (type)
    {
    case RingType::SOLID:
      drawSolid();
      break;
    case RingType::DASHED:
      drawDashed();
      break;
    case RingType::LOAD:
      drawLoad();
      break;

    default:
      break;
    }

    if (frames > 0)
      frames--;
    else
      reset();

    return true;
  }

  void setRing(RingType ringType, CRGB ringColor)
  {
    type = ringType;
    color = ringColor;
    frames = 20;
  }

  void reset()
  {
    frames = 20;
    type = RingType::NONE;
    color = CRGB::Black;
  }

  LedConfiguration &ledConfig;

private:
  RingType type = RingType::NONE;
  CRGB color = CRGB::Black;
  uint8_t frames = 20;

  void drawSolid()
  {
    const uint8_t h = ledConfig.getHeight();
    const uint8_t w = ledConfig.getWidth();
    for (uint8_t x = 0; x < w; x++)
    {
      const uint8_t pattern = pgm_read_byte(&SOLID_PATTERN[x]);
      CRGB c = color;
      c.nscale8(pattern);
      ledConfig.setPixColorXY(x, h - 1, c);
    }
  }

  void drawDashed()
  {
    const uint8_t h = ledConfig.getHeight();
    const uint8_t w = ledConfig.getWidth();
    for (uint8_t x = 0; x < w; x++)
    {
      const uint8_t pattern = pgm_read_byte(&DASHED_PATTERN[x]);
      CRGB c = color;
      c.nscale8(pattern);
      ledConfig.setPixColorXY(x, h - 1, c);
    }
  }

  void drawLoad()
  {
    const uint8_t h = ledConfig.getHeight();
    const uint8_t w = ledConfig.getWidth();
    uint8_t phase = (millis() / 100) % 8;
    for (uint8_t x = 0; x < w; x++)
    {
      uint8_t p = pgm_read_byte(&LOAD_PATTERN[(x + phase) % 8]);
      CRGB c = color;
      c.nscale8(p);
      ledConfig.setPixColorXY(x, h - 1, c);
    }
  }
};