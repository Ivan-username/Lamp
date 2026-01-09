#pragma once

#include <FastLED.h>
#include "LedConfiguration.h"
#include "EffectData.h"

// ============ BASIC EFFECT =============
class Effect
{
public:
  Effect(
      LedConfiguration &conf,
      EffSets &s,
      uint8_t &master)
      : ledConfig(conf),
        sets(s),
        masterBrightness(master) {}

  virtual ~Effect() = default;

  virtual void routine()
  {
    ledConfig.fillAll(CRGB::Black);

    CRGB dot = CRGB::Red;
    dot.nscale8_video(10); // hardcoded brightness for the dot

    ledConfig.setPixColorXY(step, 0, dot);

    step++;
    if (step >= ledConfig.getWidth())
      step = 0;
  }

  virtual void reset() { step = 0; }

protected:
  LedConfiguration &ledConfig;
  EffSets &sets;
  uint8_t &masterBrightness;

  // ===== BRIGHTNESS LOGIC =====

  inline uint8_t finalBrightness() const
  {
    return scale8_video(sets.brightness, masterBrightness);
  }

  inline CRGB applyBrightness(CRGB c) const
  {
    c.nscale8_video(finalBrightness());
    return c;
  }

  inline CHSV makeHSV(uint8_t h, uint8_t s = 255) const
  {
    return CHSV(h, s, finalBrightness());
  }

private:
  uint8_t step = 0;
};

// ============ JUST LAMP EFFECT =============
class JustLampEffect : public Effect
{
public:
  JustLampEffect(
      LedConfiguration &conf,
      EffSets &s,
      uint8_t &master) : Effect(conf, s, master) {}

  void routine() override
  {
    CRGB color = CRGB::White;
    color.nscale8_video(finalBrightness());
    ledConfig.fillAll(color);
  }

  void reset() override { return; }
};

// ============ RAINBOW HORIZONTAL EFFECT =============
class RainbowHorizontalEffect : public Effect
{
public:
  RainbowHorizontalEffect(
      LedConfiguration &conf,
      EffSets &s,
      uint8_t &master) : Effect(conf, s, master) {}

  void routine() override
  {
    for (uint16_t y = 0; y < ledConfig.getHeight(); y++)
    {
      CHSV thisColor = CHSV((byte)(hue + y * sets.scale), 255, 255);
      for (uint16_t x = 0; x < ledConfig.getWidth(); x++)
      {
        ledConfig.setPixColorXY(x, y, applyBrightness(CRGB(thisColor)));
      }
    }
    hue += 2;
  }

  void reset() override
  {
    hue = 0;
  }

private:
  uint8_t hue = 0;
};

// ============ RAINBOW VERTICAL EFFECT =============
class RainbowVerticalEffect : public Effect
{
public:
  RainbowVerticalEffect(
      LedConfiguration &conf,
      EffSets &s,
      uint8_t &master) : Effect(conf, s, master) {}

  void routine() override
  {
    for (uint16_t x = 0; x < ledConfig.getWidth(); x++)
    {
      CHSV thisColor = CHSV((byte)(hue + x * sets.scale), 255, 255);
      for (uint16_t y = 0; y < ledConfig.getHeight(); y++)
      {
        ledConfig.setPixColorXY(x, y, applyBrightness(CRGB(thisColor)));
      }
    }
    hue += 2;
  }

  void reset() override
  {
    hue = 0;
  }

private:
  uint8_t hue = 0;
};
// // ============ NOISE FIRE EFFECT =============
// class FireNoiseEffect : public Effect
// {
// public:
//   FireNoiseEffect(LedConfiguration &ledConf)
//       : Effect(ledConf) {}

//   void routine(uint8_t scale) override
//   {
//     const uint16_t w = ledConfig.getWidth();
//     const uint16_t h = ledConfig.getHeight();

//     if (h <= 1)
//       return;

//     // scale -> цветовое колесо (red -> red)
//     CHSV hsv(scale, 255, 255);
//     CRGB targetColor;
//     hsv2rgb_rainbow(hsv, targetColor);

//     const uint16_t mid = h / 2;

//     for (uint16_t y = 0; y < h; y++)
//     {
//       CRGB rowColor;

//       if (y < mid)
//       {
//         // Белый -> target
//         uint8_t t = map(y, 0, mid - 1, 0, 255);
//         rowColor = blend(CRGB::White, targetColor, t);
//       }
//       else
//       {
//         // target -> чёрный
//         uint8_t t = map(y, mid, h - 1, 0, 255);
//         rowColor = blend(targetColor, CRGB::Black, t);
//       }

//       for (uint16_t x = 0; x < w; x++)
//       {
//         ledConfig.setPixColorXY(x, y, rowColor);
//       }
//     }
//   }

//   void reset() override
//   {
//   }

// private:
//   inline uint8_t fcolor(uint8_t x)
//   {
//     uint16_t x2 = (uint16_t)x * x;
//     return (uint8_t)((x2 * x) >> 8);
//   }
// };

// // ============ GYVER FIRE EFFECT =============
// these values are substracetd from the generated values to give a shape to the animation
// const unsigned char valueMask[8][16] PROGMEM = {
//     {32, 0, 0, 0, 0, 0, 0, 32, 32, 0, 0, 0, 0, 0, 0, 32},
//     {64, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0, 0, 0, 64},
//     {96, 32, 0, 0, 0, 0, 32, 96, 96, 32, 0, 0, 0, 0, 32, 96},
//     {128, 64, 32, 0, 0, 32, 64, 128, 128, 64, 32, 0, 0, 32, 64, 128},
//     {160, 96, 64, 32, 32, 64, 96, 160, 160, 96, 64, 32, 32, 64, 96, 160},
//     {192, 128, 96, 64, 64, 96, 128, 192, 192, 128, 96, 64, 64, 96, 128, 192},
//     {255, 160, 128, 96, 96, 128, 160, 255, 255, 160, 128, 96, 96, 128, 160, 255},
//     {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}};

// // these are the hues for the fire,
// // should be between 0 (red) to about 25 (yellow)
// const unsigned char hueMask[8][16] PROGMEM = {
//     {1, 11, 19, 25, 25, 22, 11, 1, 1, 11, 19, 25, 25, 22, 11, 1},
//     {1, 8, 13, 19, 25, 19, 8, 1, 1, 8, 13, 19, 25, 19, 8, 1},
//     {1, 8, 13, 16, 19, 16, 8, 1, 1, 8, 13, 16, 19, 16, 8, 1},
//     {1, 5, 11, 13, 13, 13, 5, 1, 1, 5, 11, 13, 13, 13, 5, 1},
//     {1, 5, 11, 11, 11, 11, 5, 1, 1, 5, 11, 11, 11, 11, 5, 1},
//     {0, 1, 5, 8, 8, 5, 1, 0, 0, 1, 5, 8, 8, 5, 1, 0},
//     {0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}};
// class GyverFireEffect : public Effect
// {
// public:
//   GyverFireEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

//   void routine(uint8_t scale) override
//   {
//     if (pcnt >= 100)
//     {
//       shiftUp();
//       generateLine();
//       pcnt = 0;
//     }
//     drawFrame(scale);
//     pcnt += 30;
//   }

//   void reset() override
//   {
//     ledConfig.fillAll(CRGB::Black);
//   }

// private:
//   void generateLine()
//   {
//     for (uint8_t x = 0; x < ledConfig.getWidth(); x++)
//     {
//       line[x] = random(64, 255);
//     }
//   }
//   void shiftUp()
//   {
//     for (uint8_t y = ledConfig.getHeight() - 1; y > 0; y--)
//     {
//       for (uint8_t x = 0; x < ledConfig.getWidth(); x++)
//       {
//         uint8_t newX = x;
//         if (x > 15)
//           newX = x - 15;
//         if (y > 7)
//           continue;
//         matrixValue[y][newX] = matrixValue[y - 1][newX];
//       }
//     }

//     for (uint8_t x = 0; x < ledConfig.getWidth(); x++)
//     {
//       uint8_t newX = x;
//       if (x > 15)
//         newX = x - 15;
//       matrixValue[0][newX] = line[newX];
//     }
//   }

//   void drawFrame(uint8_t scale)
//   {
//     int nextv;

//     // each row interpolates with the one before it
//     for (unsigned char y = ledConfig.getHeight() - 1; y > 0; y--)
//     {
//       for (unsigned char x = 0; x < ledConfig.getWidth(); x++)
//       {
//         uint8_t newX = x;
//         if (x > 15)
//           newX = x - 15;
//         if (y < 8)
//         {
//           nextv =
//               (((100.0 - pcnt) * matrixValue[y][newX] + pcnt * matrixValue[y - 1][newX]) / 100.0) - pgm_read_byte(&(valueMask[y][newX]));

//           CRGB color = CHSV(
//               scale * 2.5 + pgm_read_byte(&(hueMask[y][newX])), // H
//               255,                                              // S
//               (uint8_t)max(0, nextv)                            // V
//           );

//           ledConfig.setPixColorXY(x, y, color);
//         }
//         else if (y == 8 && SPARKLES)
//         {
//           if (random(0, 20) == 0 && ledConfig.getPixColorXY(x, y - 1) != CRGB(CRGB::Black))
//             ledConfig.setPixColorXY(x, y, ledConfig.getPixColorXY(x, y - 1));
//           else
//             ledConfig.setPixColorXY(x, y, 0);
//         }
//         else if (SPARKLES)
//         {

//           // старая версия для яркости
//           if (ledConfig.getPixColorXY(x, y - 1) != CRGB(CRGB::Black))
//             ledConfig.setPixColorXY(x, y, ledConfig.getPixColorXY(x, y - 1));
//           else
//             ledConfig.setPixColorXY(x, y, 0);
//         }
//       }
//     }

//     // first row interpolates with the "next" line
//     for (unsigned char x = 0; x < ledConfig.getWidth(); x++)
//     {
//       uint8_t newX = x;
//       if (x > 15)
//         newX = x - 15;
//       CRGB color = CHSV(
//           scale * 2.5 + pgm_read_byte(&(hueMask[0][newX])),                              // H
//           255,                                                                           // S
//           (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
//       );
//       ledConfig.setPixColorXY(newX, 0, color);
//     }
//   }

//   bool SPARKLES = true;
//   unsigned char matrixValue[8][16];
//   unsigned char line[16];
//   int pcnt = 0;
// };

// // ============ RAINBOW VERTICAL EFFECT =============
// class RainbowVerticalEffect : public Effect
// {
// public:
//   RainbowVerticalEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

//   void routine(uint8_t scale) override
//   {
//     for (uint8_t y = 0; y < ledConfig.getHeight(); y++)
//     {
//       CHSV thisColor = CHSV((byte)(hue + y * scale), 255, 255);
//       for (uint8_t x = 0; x < ledConfig.getWidth(); x++)
//       {
//         ledConfig.setPixColorXY(x, y, thisColor);
//       }
//     }
//     hue += 2;
//   }

//   void reset() override
//   {
//     hue = 0;
//     ledConfig.fillAll(CRGB::Black);
//   }

// private:
//   uint8_t hue = 0;
// };

// // ============ SNOWFAll EFFECT =============
// class SnowfallEffect : public Effect
// {
// public:
//   SnowfallEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

//   void routine(uint8_t scale) override
//   {

//     uint8_t height = ledConfig.getHeight();
//     uint8_t width = ledConfig.getWidth();

//     for (byte x = 0; x < width; x++)
//     {

//       for (byte y = 0; y < height - 1; y++)
//       {
//         ledConfig.setPixColorXY(x, y, ledConfig.getPixColorXY(x, y + 1));
//       }
//     }

//     for (byte x = 0; x < width; x++)
//     {
//       // заполняем случайно верхнюю строку
//       // а также не даём двум блокам по вертикали вместе быть
//       if (ledConfig.getPixColorXY(x, height - 2) == CRGB(CRGB::Black) && (random(0, scale) == 0))
//         ledConfig.setPixColorXY(x, height - 1, 0xE0FFFF - 0x101010 * random(0, 4));
//       else
//         ledConfig.setPixColorXY(x, height - 1, 0x000000);
//     }
//   }

//   void reset() override
//   {
//     ledConfig.fillAll(CRGB::Black);
//   }

// private:
// };