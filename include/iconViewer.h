#pragma once

#include "config.h"
#include "lampLedUtils.h"

const uint8_t staIcon[8][8] PROGMEM = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

const uint8_t apIcon[8][8] PROGMEM = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 0, 0, 1},
    {0, 0, 0, 1, 0, 0, 0, 1},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

void iconAnimation(const uint8_t *icon, CRGB color)
{
  const uint8_t startX = WIDTH / 2 - 4;
  const uint8_t startY = HEIGHT / 2 - 4;

  const uint8_t ICON_SIZE = 8;

  for (uint8_t y = 0; y < ICON_SIZE; y++)
    for (uint8_t x = 0; x < ICON_SIZE; x++)
      setPixColorXY(startX + x, startY + y,
                    pgm_read_byte(icon + (7 - y) * 8 + x) ? color : CRGB::Black);

  FastLED.setBrightness(100);
  FastLED.show();
  delay(1000);
}