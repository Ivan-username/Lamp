#pragma once

#include "config.h"
#include "lampLedUtils.h"

#define ICON_SIZE 8

const uint8_t logoIcon[ICON_SIZE] PROGMEM = // example in BIN
    {
        0b01111111,
        0b10000010,
        0b00000010,
        0b00000010,
        0b00000010,
        0b00000010,
        0b10000010,
        0b01111100,
};
const uint8_t staIcon[ICON_SIZE] PROGMEM = { // example in HEX
    0x18, 0x66, 0x81, 0x3C,
    0x42, 0x18, 0x18, 0x00};
const uint8_t apIcon[ICON_SIZE] PROGMEM = {
    0x00, 0x70, 0x88, 0x8E,
    0x71, 0x11, 0x0E, 0x00};
const uint8_t httpIcon[ICON_SIZE] PROGMEM = {
    0x3C, 0x5A, 0xBD, 0xE7,
    0xBD, 0xE7, 0x5A, 0x3C};
const uint8_t webSocketIcon[ICON_SIZE] PROGMEM = {
    0x00, 0x24, 0x42, 0x99,
    0x99, 0x42, 0x24, 0x00};
const uint8_t rebootIcon[ICON_SIZE] PROGMEM = {
    0x08, 0x3C, 0x48, 0x40,
    0x42, 0x42, 0x3C, 0x00};

void iconAnimation(const uint8_t *icon, CRGB color, uint16_t duration)
{
  const uint8_t startX = WIDTH / 2 - 4;
  const uint8_t startY = HEIGHT / 2 - 4;

  FastLED.clear();

  for (uint8_t row = 0; row < ICON_SIZE; row++)
  {
    uint8_t line = pgm_read_byte(icon + (ICON_SIZE - 1 - row));

    for (uint8_t col = 0; col < ICON_SIZE; col++)
    {
      // Проверяем бит: 7-й бит — первый пиксель
      bool pixel = line & (1 << (7 - col));

      setPixColorXY(
          startX + col,
          startY + row,
          pixel ? color : CRGB::Black);
    }
  }

  FastLED.show();
  yield();

  const uint16_t steps = 200;
  const float stepTime = (float)duration / steps;

  for (uint16_t i = 0; i < steps; i++)
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