#pragma once

#include "config.h"

inline uint16_t getPixIndex(int8_t _x, int8_t _y)
{
    const uint8_t width = WIDTH / 2;

    switch (MATRIX_TYPE)
    {
    // ===== Type 0: rows =====
    case 0:
        return _y * WIDTH + _x;

    // ===== Type 1: snake =====
    case 1:
    {
        uint16_t base = _y * WIDTH;
        return (_y & 1) ? (base + (WIDTH - 1 - _x)) : (base + _x);
    }

    // ===== Type 2: (nxn)x2 snake =====
    case 2:
    {
        uint16_t row = _y * width;
        uint16_t panelOffset = width * HEIGHT;

        bool isSecond = (_x >= width);
        uint8_t localX = isSecond ? (_x - width) : _x;

        uint16_t index = row + ((_y & 1) ? (width - 1 - localX) : localX);

        return isSecond ? (index + panelOffset) : index;
    }
    }

    return 0;
}

inline uint32_t getPixColor(int index)
{
    if (index < 0 || index > LED_AMOUNT - 1)
        return 0;
    return (((uint32_t)leds[index].r << 16) | ((long)leds[index].g << 8) | (long)leds[index].b);
}

inline uint32_t getPixColorXY(int8_t x, int8_t y)
{
    return getPixColor(getPixIndex(x, y));
}

inline void fillAll(CRGB color)
{
    FOR_U8_I(0, LED_AMOUNT)
    {
        leds[i] = color;
    }
}

inline void setPixColorXY(int8_t x, int8_t y, CRGB color)
{
    if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1)
        return;
    leds[getPixIndex(x, y)] = color;
}
