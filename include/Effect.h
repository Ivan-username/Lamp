#pragma once

#include "config.h"
#include "LedConfiguration.h"

// ============ BASIC EFFECT =============
class Effect
{
public:
  Effect(LedConfiguration &ledConf) : ledConfig(ledConf) {}
  virtual ~Effect() = default;

  virtual void routine(uint8_t scale)
  {
    sc = scale;
    runTestDot();
  };

  virtual void reset() { resetDot(); };

protected:
  LedConfiguration &ledConfig;

  void runTestDot()
  {
    ledConfig.fillAll(CRGB::Black);
    ledConfig.setPixColorXY(testX, 0, CRGB::Red);

    testX++;
    if (testX >= ledConfig.getWidth())
      testX = 0;
  }

  void resetDot() { testX = 0; }

  uint16_t testX = 0;
  uint8_t sc;
};

// ============ JUST LAMP EFFECT =============
class JustLampEffect : public Effect
{
public:
  JustLampEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

  void routine(uint8_t scale) override
  {
    // scale: 1 - 50 → цветовая температура 2000K - 6500K
    uint16_t kelvin = map(scale, 1, 50, 2000, 6500);

    CRGB color = kelvinToCRGB(kelvin);

    // заливаем всю матрицу
    ledConfig.fillAll(color);
  }

  void reset() override
  {
    ledConfig.fillAll(CRGB::Black);
  }

private:
  // Конвертация K → CRGB (приближённая, без FastLED ColorTemperatureToRGB)
  CRGB kelvinToCRGB(uint16_t kelvin)
  {
    float temperature = kelvin / 100.0f;
    float r, g, b;

    // Красный
    if (temperature <= 66)
      r = 255;
    else
      r = 329.698727446f * powf(temperature - 60, -0.1332047592f);

    // Зеленый
    if (temperature <= 66)
      g = 99.4708025861f * logf(temperature) - 161.1195681661f;
    else
      g = 288.1221695283f * powf(temperature - 60, -0.0755148492f);

    // Синий
    if (temperature >= 66)
      b = 255;
    else if (temperature <= 19)
      b = 0;
    else
      b = 138.5177312231f * logf(temperature - 10) - 305.0447927307f;

    // Ограничиваем [0,255]
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    return CRGB(uint8_t(r), uint8_t(g), uint8_t(b));
  }
};

// ============ RAINBOW HORIZONTAL EFFECT =============
class RainbowHorizontalEffect : public Effect
{
public:
  RainbowHorizontalEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

  void routine(uint8_t scale) override
  {
    for (uint8_t y = 0; y < ledConfig.getWidth(); y++)
    {
      CHSV thisColor = CHSV((byte)(hue + y * scale), 255, 255);
      for (uint8_t x = 0; x < ledConfig.getHeight(); x++)
        ledConfig.setPixColorXY(y, x, thisColor);
    }
    hue += 2;
  }

  void reset() override
  {
    hue = 0;
    ledConfig.fillAll(CRGB::Black);
  }

private:
  uint8_t hue = 0;
};

// ============ RAINBOW VERTICAL EFFECT =============
class RainbowVerticalEffect : public Effect
{
public:
  RainbowVerticalEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

  void routine(uint8_t scale) override
  {
    for (uint8_t y = 0; y < ledConfig.getHeight(); y++)
    {
      CHSV thisColor = CHSV((byte)(hue + y * scale), 255, 255);
      for (uint8_t x = 0; x < ledConfig.getWidth(); x++)
      {
        ledConfig.setPixColorXY(x, y, thisColor);
      }
    }
    hue += 2;
  }

  void reset() override
  {
    hue = 0;
    ledConfig.fillAll(CRGB::Black);
  }

private:
  uint8_t hue = 0;
};

// ============ RAINBOW VERTICAL EFFECT =============
class SnowfallEffect : public Effect
{
public:
  SnowfallEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

  void routine(uint8_t scale) override
  {

    uint8_t height = ledConfig.getHeight();
    uint8_t width = ledConfig.getWidth();

    for (byte x = 0; x < width; x++)
    {

      for (byte y = 0; y < height - 1; y++)
      {
        ledConfig.setPixColorXY(x, y, ledConfig.getPixColorXY(x, y + 1));
      }
    }

    for (byte x = 0; x < width; x++)
    {
      // заполняем случайно верхнюю строку
      // а также не даём двум блокам по вертикали вместе быть
      if (ledConfig.getPixColorXY(x, height - 2) == CRGB(CRGB::Black) && (random(0, scale) == 0))
        ledConfig.setPixColorXY(x, height - 1, 0xE0FFFF - 0x101010 * random(0, 4));
      else
        ledConfig.setPixColorXY(x, height - 1, 0x000000);
    }
  }

  void reset() override
  {
    ledConfig.fillAll(CRGB::Black);
  }

private:
};