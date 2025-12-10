#pragma once
#include "config.h"

class LedConfiguration
{
public:
    LedConfiguration(CRGB *extLeds, uint16_t w, uint16_t h)
        : _leds(extLeds), _width(w), _height(h)
    {
        _ledsAmount = w * h;
    }

    virtual ~LedConfiguration() = default;

    virtual uint16_t getPixIndex(int16_t x, int16_t y) const = 0;

    inline CRGB getPixColorXY(int16_t x, int16_t y) const
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
            return CRGB::Black;

        return _leds[getPixIndex(x, y)];
    }

    inline void setPixColorXY(int16_t x, int16_t y, CRGB color)
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
            return;

        _leds[getPixIndex(x, y)] = color;
    }

    inline void fillAll(CRGB color)
    {
        for (uint16_t i = 0; i < _ledsAmount; i++)
            _leds[i] = color;
    }

    inline uint16_t getWidth() const { return _width; }
    inline uint16_t getHeight() const { return _height; }

    CRGB *_leds;

protected:
    uint16_t _width = 0;
    uint16_t _height = 0;
    uint16_t _ledsAmount = 0;
};

// ============ ROW MATRIX =============

class RowMatrix : public LedConfiguration
{
public:
    RowMatrix(CRGB *extLeds, uint16_t w, uint16_t h)
        : LedConfiguration(extLeds, w, h) {}

    uint16_t getPixIndex(int16_t x, int16_t y) const override
    {
        return y * _width + x;
    }
};

// ============ SNAKE MATRIX =============

class SnakeMatrix : public LedConfiguration
{
public:
    SnakeMatrix(CRGB *extLeds, uint16_t w, uint16_t h)
        : LedConfiguration(extLeds, w, h) {}

    uint16_t getPixIndex(int16_t x, int16_t y) const override
    {
        uint16_t base = y * _width;
        return (y & 1) ? (base + (_width - 1 - x)) : (base + x);
    }
};

// ============ DOUBLE PANEL SNAKE =============

class DoublePanelSnakeMatrix : public LedConfiguration
{
public:
    DoublePanelSnakeMatrix(CRGB *extLeds, uint16_t w, uint16_t h)
        : LedConfiguration(extLeds, w, h)
    {
        _halfWidth = w / 2;
        _panelSize = _halfWidth * h;
    }

    uint16_t getPixIndex(int16_t x, int16_t y) const override
    {
        bool isSecond = (x >= _halfWidth);
        int localX = isSecond ? (x - _halfWidth) : x;

        uint16_t rowOffset = y * _halfWidth;

        uint16_t index = rowOffset +
                         ((y & 1) ? (_halfWidth - 1 - localX) : localX);

        return isSecond ? index + _panelSize : index;
    }

private:
    uint16_t _halfWidth = 0;
    uint16_t _panelSize = 0;
};
