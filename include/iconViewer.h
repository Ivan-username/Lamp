#pragma once

#include "config.h"
#include "lampLedUtils.h"

//  {{0, 0, 0, 0, 0, 0, 0, 0},
//   {0, 0, 0, 0, 0, 0, 0, 0},
//   {0, 0, 0, 0, 0, 0, 0, 0},
//   {0, 0, 0, 0, 0, 0, 0, 0},
//   {0, 0, 0, 0, 0, 0, 0, 0},
//   {0, 0, 0, 0, 0, 0, 0, 0},
//   {0, 0, 0, 0, 0, 0, 0, 0},
//   {0, 0, 0, 0, 0, 0, 0, 0}};

const uint8_t logoIcon[8][8] PROGMEM =
    {{0, 0, 1, 1, 1, 1, 1, 0},
     {0, 1, 0, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0, 1, 0, 0},
     {0, 0, 0, 1, 1, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0}};

const uint8_t staIcon[8][8] PROGMEM =
    {{0, 0, 0, 1, 1, 0, 0, 0},
     {0, 1, 1, 0, 0, 1, 1, 0},
     {1, 0, 0, 0, 0, 0, 0, 1},
     {0, 0, 1, 1, 1, 1, 0, 0},
     {0, 1, 0, 0, 0, 0, 1, 0},
     {0, 0, 0, 1, 1, 0, 0, 0},
     {0, 0, 0, 1, 1, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0}};

const uint8_t apIcon[8][8] PROGMEM =
    {{0, 0, 0, 0, 0, 0, 0, 0},
     {0, 1, 1, 1, 0, 0, 0, 0},
     {1, 0, 0, 0, 1, 0, 0, 0},
     {1, 0, 0, 0, 1, 1, 1, 0},
     {0, 1, 1, 1, 0, 0, 0, 1},
     {0, 0, 0, 1, 0, 0, 0, 1},
     {0, 0, 0, 0, 1, 1, 1, 0},
     {0, 0, 0, 0, 0, 0, 0, 0}};

const uint8_t httpIcon[8][8] PROGMEM =
    {{0, 0, 1, 1, 1, 1, 0, 0},
     {0, 1, 0, 1, 1, 0, 1, 0},
     {1, 0, 1, 1, 1, 1, 0, 1},
     {1, 1, 1, 0, 0, 1, 1, 1},
     {1, 0, 1, 1, 1, 1, 0, 1},
     {1, 1, 1, 0, 0, 1, 1, 1},
     {0, 1, 0, 1, 1, 0, 1, 0},
     {0, 0, 1, 1, 1, 1, 0, 0}};

const uint8_t webSocketIcon[8][8] PROGMEM =
    {{0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 1, 0, 0, 1, 0, 0},
     {0, 1, 0, 0, 0, 0, 1, 0},
     {1, 0, 0, 1, 1, 0, 0, 1},
     {1, 0, 0, 1, 1, 0, 0, 1},
     {0, 1, 0, 0, 0, 0, 1, 0},
     {0, 0, 1, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0}};

const uint8_t rebootIcon[8][8] PROGMEM =
    {{0, 0, 0, 0, 1, 0, 0, 0},
     {0, 0, 1, 1, 1, 1, 0, 0},
     {0, 1, 0, 0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0, 0, 0, 0},
     {0, 1, 0, 0, 0, 0, 1, 0},
     {0, 1, 0, 0, 0, 0, 1, 0},
     {0, 0, 1, 1, 1, 1, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0}};

void iconAnimation(const uint8_t *icon, CRGB color, uint16_t duration)
{
  const uint8_t startX = WIDTH / 2 - 4;
  const uint8_t startY = HEIGHT / 2 - 4;
  const uint8_t ICON_SIZE = 8;

  FastLED.clear();

  FOR_U8_I(0, ICON_SIZE)
  {
    FOR_U8_J(0, ICON_SIZE)
    {
      setPixColorXY(startX + j, startY + i,
                    pgm_read_byte(icon + (7 - i) * 8 + j) ? color : CRGB::Black);
    }
  }

  FastLED.show();
  yield();

  const uint16_t steps = 200;
  const float stepTime = (float)duration / steps;

  FOR_U16_I(0, steps)
  {
    float t = (float)i / (steps - 1);
    float wave = sin(PI * t);

    uint8_t brightness = (uint8_t)(wave * 50);

    FastLED.setBrightness(brightness);
    FastLED.show();

    delay(stepTime);
  }
  FastLED.clear();
}