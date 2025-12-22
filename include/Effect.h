#pragma once

#include "config.h"
#include "LedConfiguration.h"

// these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM = {
    {32, 0, 0, 0, 0, 0, 0, 32, 32, 0, 0, 0, 0, 0, 0, 32},
    {64, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0, 0, 0, 64},
    {96, 32, 0, 0, 0, 0, 32, 96, 96, 32, 0, 0, 0, 0, 32, 96},
    {128, 64, 32, 0, 0, 32, 64, 128, 128, 64, 32, 0, 0, 32, 64, 128},
    {160, 96, 64, 32, 32, 64, 96, 160, 160, 96, 64, 32, 32, 64, 96, 160},
    {192, 128, 96, 64, 64, 96, 128, 192, 192, 128, 96, 64, 64, 96, 128, 192},
    {255, 160, 128, 96, 96, 128, 160, 255, 255, 160, 128, 96, 96, 128, 160, 255},
    {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}};

// these are the hues for the fire,
// should be between 0 (red) to about 25 (yellow)
const unsigned char hueMask[8][16] PROGMEM = {
    {1, 11, 19, 25, 25, 22, 11, 1, 1, 11, 19, 25, 25, 22, 11, 1},
    {1, 8, 13, 19, 25, 19, 8, 1, 1, 8, 13, 19, 25, 19, 8, 1},
    {1, 8, 13, 16, 19, 16, 8, 1, 1, 8, 13, 16, 19, 16, 8, 1},
    {1, 5, 11, 13, 13, 13, 5, 1, 1, 5, 11, 13, 13, 13, 5, 1},
    {1, 5, 11, 11, 11, 11, 5, 1, 1, 5, 11, 11, 11, 11, 5, 1},
    {0, 1, 5, 8, 8, 5, 1, 0, 0, 1, 5, 8, 8, 5, 1, 0},
    {0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}};

// ============ BASIC EFFECT =============
class Effect
{
public:
  Effect(LedConfiguration &ledConf) : ledConfig(ledConf) {}
  virtual ~Effect() = default;

  virtual void routine(uint8_t scale)
  {
    runTestDot();
  };

  virtual void reset() { resetDot(); };

protected:
  void runTestDot()
  {
    ledConfig.fillAll(CRGB::Black);
    ledConfig.setPixColorXY(testX, 0, CRGB::Red);

    testX++;
    if (testX >= ledConfig.getWidth())
      testX = 0;
  }

  void resetDot() { testX = 0; }

  LedConfiguration &ledConfig;
  uint16_t testX = 0;
};

// ============ JUST LAMP EFFECT =============
class JustLampEffect : public Effect
{
public:
  JustLampEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

  void routine(uint8_t scale) override
  {
    // scale: 1 - 50 → 2500K - 6500K (ламповый диапазон)
    uint16_t kelvin = map(scale, 1, 50, 2500, 6500);

    CRGB color = kelvinToCRGB(kelvin);
    ledConfig.fillAll(color);
  }

  void reset() override
  {
    ledConfig.fillAll(CRGB::Black);
  }

private:
  CRGB kelvinToCRGB(uint16_t kelvin)
  {
    float temperature = kelvin / 100.0f;
    float r, g, b;

    // Red
    if (temperature <= 66)
      r = 255;
    else
      r = 329.698727446f * powf(temperature - 60, -0.1332047592f);

    // Green
    if (temperature <= 66)
      g = 99.4708025861f * logf(temperature) - 161.1195681661f;
    else
      g = 288.1221695283f * powf(temperature - 60, -0.0755148492f);

    // Blue
    if (temperature >= 66)
      b = 255;
    else if (temperature <= 19)
      b = 0;
    else
      b = 138.5177312231f * logf(temperature - 10) - 305.0447927307f;

    // ==== КОРРЕКЦИЯ ДЛЯ LED ====
    if (kelvin < 3500)
    {
      float k = (3500.0f - kelvin) / 1000.0f; // 0..1
      g *= (1.0f - 0.15f * k);                // убираем зелень
    }

    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    return CRGB(uint8_t(r), uint8_t(g), uint8_t(b));
  }
};

// ============ NOISE FIRE EFFECT =============
class FireNoiseEffect : public Effect
{
public:
  FireNoiseEffect(LedConfiguration &ledConf)
      : Effect(ledConf) {}

  void routine(uint8_t scale) override
  {
    uint8_t width = ledConfig.getWidth();
    uint8_t height = ledConfig.getHeight();

    // Высота пламени ≈ половина ширины
    uint8_t fireHeight = min<uint8_t>(height, width / 2);

    // Цвет пламени: от красного к оранжево-жёлтому
    // Hue: 0 (красный) → ~32 (оранжевый)
    uint8_t baseHue = map(scale, 1, 50, 0, 32);

    // Масштаб шума (размер языков)
    uint8_t noiseScale = 30;

    // Гасим всё
    ledConfig.fillAll(CRGB::Black);

    // Основное пламя
    for (uint8_t y = 0; y < fireHeight; y++)
    {
      for (uint8_t x = 0; x < width; x++)
      {
        // Координаты в шумовом пространстве
        uint16_t nx = x * noiseScale;
        uint16_t ny = y * noiseScale;

        // Шум, текущий по времени
        uint8_t n = inoise8(nx, ny + time);

        // Усиливаем низ и гасим верх
        uint8_t heat = qsub8(n, y * (255 / fireHeight));

        if (heat < 20)
          continue;

        // Яркость и насыщенность
        uint8_t bri = scale8(heat, 240);
        uint8_t sat = 255;

        // Немного гуляющий hue для живости
        uint8_t hue = baseHue + scale8(heat, 10);

        // Рисуем снизу вверх
        uint8_t drawY = fireHeight - 1 - y;
        ledConfig.setPixColorXY(x, drawY, CHSV(hue, sat, bri));
      }
    }

    // ===== УГОЛЬКИ =====
    if (random8() < 40)
    {
      uint8_t ex = random8(width);
      uint8_t ey = fireHeight + random8(2);

      if (ey < height)
      {
        ledConfig.setPixColorXY(
            ex,
            ey,
            CHSV(baseHue + 10, 200, 255));
      }
    }

    time += 2;
  }

  void reset() override
  {
    time = 0;
    ledConfig.fillAll(CRGB::Black);
  }

private:
  uint16_t time = 0;
};

// ============ GYVER FIRE EFFECT =============
class GyverFireEffect : public Effect
{
public:
  GyverFireEffect(LedConfiguration &ledConf) : Effect(ledConf) {}

  void routine(uint8_t scale) override
  {
    if (pcnt >= 100)
    {
      shiftUp();
      generateLine();
      pcnt = 0;
    }
    drawFrame(scale);
    pcnt += 30;
  }

  void reset() override
  {
    ledConfig.fillAll(CRGB::Black);
  }

private:
  void generateLine()
  {
    for (uint8_t x = 0; x < ledConfig.getWidth(); x++)
    {
      line[x] = random(64, 255);
    }
  }
  void shiftUp()
  {
    for (uint8_t y = ledConfig.getHeight() - 1; y > 0; y--)
    {
      for (uint8_t x = 0; x < ledConfig.getWidth(); x++)
      {
        uint8_t newX = x;
        if (x > 15)
          newX = x - 15;
        if (y > 7)
          continue;
        matrixValue[y][newX] = matrixValue[y - 1][newX];
      }
    }

    for (uint8_t x = 0; x < ledConfig.getWidth(); x++)
    {
      uint8_t newX = x;
      if (x > 15)
        newX = x - 15;
      matrixValue[0][newX] = line[newX];
    }
  }

  void drawFrame(uint8_t scale)
  {
    int nextv;

    // each row interpolates with the one before it
    for (unsigned char y = ledConfig.getHeight() - 1; y > 0; y--)
    {
      for (unsigned char x = 0; x < ledConfig.getWidth(); x++)
      {
        uint8_t newX = x;
        if (x > 15)
          newX = x - 15;
        if (y < 8)
        {
          nextv =
              (((100.0 - pcnt) * matrixValue[y][newX] + pcnt * matrixValue[y - 1][newX]) / 100.0) - pgm_read_byte(&(valueMask[y][newX]));

          CRGB color = CHSV(
              scale * 2.5 + pgm_read_byte(&(hueMask[y][newX])), // H
              255,                                              // S
              (uint8_t)max(0, nextv)                            // V
          );

          ledConfig.setPixColorXY(x, y, color);
        }
        else if (y == 8 && SPARKLES)
        {
          if (random(0, 20) == 0 && ledConfig.getPixColorXY(x, y - 1) != CRGB(CRGB::Black))
            ledConfig.setPixColorXY(x, y, ledConfig.getPixColorXY(x, y - 1));
          else
            ledConfig.setPixColorXY(x, y, 0);
        }
        else if (SPARKLES)
        {

          // старая версия для яркости
          if (ledConfig.getPixColorXY(x, y - 1) != CRGB(CRGB::Black))
            ledConfig.setPixColorXY(x, y, ledConfig.getPixColorXY(x, y - 1));
          else
            ledConfig.setPixColorXY(x, y, 0);
        }
      }
    }

    // first row interpolates with the "next" line
    for (unsigned char x = 0; x < ledConfig.getWidth(); x++)
    {
      uint8_t newX = x;
      if (x > 15)
        newX = x - 15;
      CRGB color = CHSV(
          scale * 2.5 + pgm_read_byte(&(hueMask[0][newX])),                              // H
          255,                                                                           // S
          (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
      );
      ledConfig.setPixColorXY(newX, 0, color);
    }
  }

  bool SPARKLES = true;
  unsigned char matrixValue[8][16];
  unsigned char line[16];
  int pcnt = 0;
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

// ============ SNOWFAll EFFECT =============
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