#pragma once

#include "config.h"
#include "LedConfiguration.h"
#include "LampState.h"

const uint8_t a_frames[1][8] PROGMEM = {{0b00000000,
                                         0b00000000,
                                         0b00000000,
                                         0b00011000,
                                         0b00011000,
                                         0b00000000,
                                         0b00000000,
                                         0b00000000}};

class Animation
{
public:
  Animation(LedConfiguration &ledConf) : ledConfig(ledConf) {}
  virtual ~Animation() = default;

  virtual bool routine(CRGB color)
  {
    return runTestAnimation(color);
  };

protected:
  bool runTestAnimation(CRGB color)
  {
    if (frame == TOTAL_FRAMES)
      return false;

    ledConfig.fillAll(CRGB::Black);

    CRGB calcColor(
        (color * brightness) >> 8,
        (color * brightness) >> 8,
        (color * brightness) >> 8);

    int16_t offsetX = (ledConfig.getWidth() - 8) / 2;
    int16_t offsetY = (ledConfig.getHeight() - 8) / 2;

    for (uint8_t y = 0; y < 8; y++)
    {
      uint8_t row = pgm_read_byte(&a_frames[0][y]);

      for (uint8_t x = 0; x < 8; x++)
      {
        if (row & (1 << (7 - x)))
        {
          ledConfig.setPixColorXY(
              x + offsetX,
              y + offsetY,
              calcColor);
        }
      }
    }

    if (frame < HALF_FRAMES)
      brightness += brStep;
    else
      brightness += brStep;

    frame++;
    return true;
  }

private:
  const uint8_t duration = 1;
  const uint8_t FPS = 30;
  const uint8_t TOTAL_FRAMES = FPS * duration;
  const uint8_t HALF_FRAMES = FPS / 2;
  const uint8_t MAX_BRIGHT = 128;
  const uint8_t brStep = 256 / HALF_FRAMES;

  uint8_t brightness = 0;
  uint8_t frame = 0;

  LedConfiguration &ledConfig;
};