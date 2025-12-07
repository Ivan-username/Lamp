#pragma once

#include "config.h"
#include "Renderer.h"

// ============ BASIC EFFECT =============
class Effect
{
public:
  Effect(IRenderer *renderer) : _renderer(renderer) {}
  virtual ~Effect() = default;

  // virtual void reset() = 0;

  virtual void setBrightness(uint8_t b)
  {
    _brightness = b;
  }
  virtual void setSpeed(uint8_t s)
  {
    _speed = s;
  }
  virtual void setScale(uint8_t sc)
  {
    _scale = sc;
  }

  virtual void routine()
  {
    runTestDot();
  };

  uint8_t _brightness = 50;
  uint8_t _speed = 30;
  uint8_t _scale = 40;

protected:
  IRenderer *_renderer;

  void runTestDot()
  {
    _renderer->fill(CRGB::Black);
    _renderer->setPixel(testX, 0, CRGB::Red);

    testX++;
    if (testX >= _renderer->width())
      testX = 0;
  }

  uint16_t testX = 0;
};

// ============ RAINBOW EFFECT =============
class RainbowEffect : public Effect
{
public:
  RainbowEffect(IRenderer *r) : Effect(r) {}

  void routine() override
  {

    static uint8_t hue = 0;
    for (int y = 0; y < _renderer->height(); y++)
    {
      CHSV thisColor = CHSV((byte)(hue + y * _scale), 255, 255);
      for (int x = 0; x < _renderer->width(); x++)
      {
        _renderer->setPixel(x, y, thisColor);
      }
    }
    hue++;
  }
};