#pragma once

#include "config.h"
#include "Matrix.h"

class IRenderer
{
public:
  virtual void setPixel(uint16_t x, uint16_t y, CRGB color) = 0;
  virtual CRGB getPixel(uint16_t x, uint16_t y) = 0;
  virtual void fill(CRGB color) = 0;
  virtual uint16_t width() const = 0;
  virtual uint16_t height() const = 0;
  virtual ~IRenderer() = default;
};

class MatrixRenderer : public IRenderer
{
public:
  MatrixRenderer(Matrix *m) : m(m) {}

  void setPixel(uint16_t x, uint16_t y, CRGB color) override
  {
    uint16_t index = m->getPixIndex(x, y);
    m->_leds[index] = color;
  }

  CRGB getPixel(uint16_t x, uint16_t y) override
  {
    uint16_t index = m->getPixIndex(x, y);
    return m->_leds[index];
  }

  void fill(CRGB color) override
  {
    m->fillAll(color);
  }

  uint16_t width() const override { return m->getWidth(); }
  uint16_t height() const override { return m->getHeight(); }

private:
  Matrix *m;
};