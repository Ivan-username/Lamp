#pragma once

#include "config.h"
#include "LedConfiguration.h"

// ============ BASIC EFFECT =============
class Effect
{
public:
  struct EffectData
  {
    uint8_t brightness = 30;
    uint8_t speed = 30;
    uint8_t scale = 30;
  } data;

  Effect(LedConfiguration &ledConf) : ledConfig(ledConf) {}
  virtual ~Effect() = default;

  virtual void setBrightness(uint8_t b) { data.brightness = b; }

  virtual void setSpeed(uint8_t s) { data.speed = s; }

  virtual void setScale(uint8_t sc) { data.scale = sc; }

  virtual void routine() { runTestDot(); };

  // virtual void reset() = 0;

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

  uint16_t testX = 0;
};

// ============ RAINBOW EFFECT =============
class RainbowEffect : public Effect
{
public:
  RainbowEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

  void routine() override
  {

    static uint8_t hue = 0;
    for (int y = 0; y < ledConfig.getHeight(); y++)
    {
      CHSV thisColor = CHSV((byte)(hue + y * data.scale), 255, 255);
      for (int x = 0; x < ledConfig.getWidth(); x++)
      {
        ledConfig.setPixColorXY(x, y, thisColor);
      }
    }
    hue++;
  }
};