#pragma once

#include <FastLED.h>

#include "LedConfiguration.h"
#include "LampState.h"

#include "DEBUG.h"

const uint8_t FPS = 20;
const uint8_t MAX_BRIGHT = 128;

const uint8_t testAnimFrames[1][8] PROGMEM = {{0b00000000,
                                               0b00000000,
                                               0b00000000,
                                               0b00011000,
                                               0b00011000,
                                               0b00000000,
                                               0b00000000,
                                               0b00000000}};

const uint8_t STAAnimFrames[1][8] PROGMEM = {{0b00000000,
                                              0b00000000,
                                              0b01110000,
                                              0b00001000,
                                              0b01100100,
                                              0b00010100,
                                              0b01010100,
                                              0b00000000}};

class Animation
{
public:
  Animation(LedConfiguration &ledConf) : ledConfig(ledConf) {}
  virtual ~Animation() = default;

  virtual bool routine(CRGB color = CRGB::White)
  {
    return runTestAnimation(color);
  };

protected:
  LedConfiguration &ledConfig;

private:
  bool runTestAnimation(CRGB color)
  {
    if (frame == TOTAL_FRAMES)
      return false;

    ledConfig.fillAll(CRGB::Black);

    int16_t offsetX = (ledConfig.getWidth() - 8) / 2;
    int16_t offsetY = (ledConfig.getHeight() - 8) / 2;

    for (uint8_t y = 0; y < 8; y++)
    {
      uint8_t row = pgm_read_byte(&testAnimFrames[0][y]);

      for (uint8_t x = 0; x < 8; x++)
      {
        if (row & (1 << (7 - x)))
        {
          ledConfig.setPixColorXY(
              x + offsetX,
              y + offsetY,
              color);
        }
      }
    }

    FastLED.setBrightness(MAX_BRIGHT);

    frame++;
    return true;
  }

  const uint8_t duration = 1;

  const uint8_t TOTAL_FRAMES = FPS * duration;
  const uint8_t HALF_FRAMES = TOTAL_FRAMES / 2;
  const uint8_t BRIGHT_STEP = MAX_BRIGHT / HALF_FRAMES;

  uint8_t frame = 0;
};

class STAAnimation : public Animation
{
public:
  STAAnimation(LedConfiguration &ledConf) : Animation(ledConf) {}

  virtual bool routine(CRGB color)
  {
    if (frame == TOTAL_FRAMES)
      return false;

    ledConfig.fillAll(CRGB::Black);

    int16_t offsetX = (ledConfig.getWidth() - 8) / 2;
    int16_t offsetY = (ledConfig.getHeight() - 8) / 2;

    for (uint8_t y = 0; y < 8; y++)
    {
      uint8_t row = pgm_read_byte(&STAAnimFrames[0][y]);

      for (uint8_t x = 0; x < 8; x++)
      {
        if (row & (1 << (7 - x)))
        {
          ledConfig.setPixColorXY(
              x + offsetX,
              y + offsetY,
              color);
        }
      }
    }

    if (frame < HALF_FRAMES)
      brightness += brStep;
    else
      brightness -= brStep;

    FastLED.setBrightness(brightness);

    frame++;
    return true;
  }

private:
  const uint8_t duration = 1;

  const uint8_t TOTAL_FRAMES = FPS * duration;
  const uint8_t HALF_FRAMES = TOTAL_FRAMES / 2;
  const uint8_t brStep = MAX_BRIGHT / HALF_FRAMES;

  uint8_t brightness = 0;
  uint8_t frame = 0;
};