#pragma once

#include "config.h"
#include "Renderer.h"

// ============ EFFECT =============
class Effect
{
public:
  Effect(IRenderer *renderer) : _renderer(renderer) {}
  virtual ~Effect() = default;

  virtual void routine()
  {
    runTestDot();
  };
  virtual void reset()
  {
    testX = 0;
    _renderer->fill(CRGB::Black);
  }

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