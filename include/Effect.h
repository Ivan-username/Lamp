#pragma once

#include "config.h"
#include "LedConfiguration.h"

// ============ BASIC EFFECT =============
class Effect
{
public:
  Effect(LedConfiguration &ledConf) : ledConfig(ledConf) {}
  virtual ~Effect() = default;

  virtual void routine(uint8_t scale) { runTestDot(); };

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
};

// ============ RAINBOW EFFECT =============
class RainbowEffect : public Effect
{
public:
  RainbowEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

  void routine(uint8_t scale) override
  {
    for (int y = 0; y < ledConfig.getHeight(); y++)
    {
      CHSV thisColor = CHSV((byte)(hue + y * scale), 255, 255);
      for (int x = 0; x < ledConfig.getWidth(); x++)
      {
        ledConfig.setPixColorXY(x, y, thisColor);
      }
    }
    hue++;
  }

  void reset() override
  {
    hue = 0;
  }

private:
  uint8_t hue = 0;
};