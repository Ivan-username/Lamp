#pragma once

#include "config.h"
#include "lampLedUtils.h"

void ledsSetup()
{
    FastLED.addLeds<LED_TYPE, LED_PIN, LED_COL_ORDER>(leds, LED_AMOUNT);
    FastLED.setBrightness(0);
    FastLED.clear(true);
}
//================= Off =================//
void offRoutine()
{
    fillAll(CRGB::Black);
}

//================= Red =================// 0
int J = 0;
boolean flag = true;
void lampRoutine()
{
    if (effectSlowStart)
    {
        effectSlowStart = false;
        J = 0;
    }
    if (J > WIDTH)
    {
        J = 0;
        flag = !flag;
    }

    FOR_U8_I(0, HEIGHT)
    {
        if (flag)
            setPixColorXY(J, i, CRGB::Red);
        else
            setPixColorXY(J, i, CRGB::Black);
    }

    J++;
}

//================= Fire =================// 1
#define SPARKLES 1 // coules
unsigned char line[WIDTH];
int pcnt = 0;
unsigned char matrixValue[8][16];
const unsigned char valueMask[8][16] PROGMEM = {
    {32, 0, 0, 0, 0, 0, 0, 32, 32, 0, 0, 0, 0, 0, 0, 32},
    {64, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0, 0, 0, 64},
    {96, 32, 0, 0, 0, 0, 32, 96, 96, 32, 0, 0, 0, 0, 32, 96},
    {128, 64, 32, 0, 0, 32, 64, 128, 128, 64, 32, 0, 0, 32, 64, 128},
    {160, 96, 64, 32, 32, 64, 96, 160, 160, 96, 64, 32, 32, 64, 96, 160},
    {192, 128, 96, 64, 64, 96, 128, 192, 192, 128, 96, 64, 64, 96, 128, 192},
    {255, 160, 128, 96, 96, 128, 160, 255, 255, 160, 128, 96, 96, 128, 160, 255},
    {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}};

const unsigned char hueMask[8][16] PROGMEM = {
    {1, 11, 19, 25, 25, 22, 11, 1, 1, 11, 19, 25, 25, 22, 11, 1},
    {1, 8, 13, 19, 25, 19, 8, 1, 1, 8, 13, 19, 25, 19, 8, 1},
    {1, 8, 13, 16, 19, 16, 8, 1, 1, 8, 13, 16, 19, 16, 8, 1},
    {1, 5, 11, 13, 13, 13, 5, 1, 1, 5, 11, 13, 13, 13, 5, 1},
    {1, 5, 11, 11, 11, 11, 5, 1, 1, 5, 11, 11, 11, 11, 5, 1},
    {0, 1, 5, 8, 8, 5, 1, 0, 0, 1, 5, 8, 8, 5, 1, 0},
    {0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}};

void generateLine()
{
    for (uint8_t x = 0; x < WIDTH; x++)
    {
        line[x] = random(64, 255);
    }
}

void shiftUp()
{
    for (uint8_t y = HEIGHT - 1; y > 0; y--)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            uint8_t newX = x;
            if (x > 15)
                newX = x - 15;
            if (y > 7)
                continue;
            matrixValue[y][newX] = matrixValue[y - 1][newX];
        }
    }

    for (uint8_t x = 0; x < WIDTH; x++)
    {
        uint8_t newX = x;
        if (x > 15)
            newX = x - 15;
        matrixValue[0][newX] = line[newX];
    }
}

void drawFrame(int pcnt)
{
    int nextv;

    for (unsigned char y = HEIGHT - 1; y > 0; y--)
    {
        for (unsigned char x = 0; x < WIDTH; x++)
        {
            uint8_t newX = x;
            if (x > 15)
                newX = x - 15;
            if (y < 8)
            {
                nextv =
                    (((100.0 - pcnt) * matrixValue[y][newX] + pcnt * matrixValue[y - 1][newX]) / 100.0) - pgm_read_byte(&(valueMask[y][newX]));

                CRGB color = CHSV(
                    modes[1].scale * 2.5 + pgm_read_byte(&(hueMask[y][newX])), // H
                    255,                                                       // S
                    (uint8_t)max(0, nextv)                                     // V
                );

                leds[getPixIndex(x, y)] = color;
            }
            else if (y == 8 && SPARKLES)
            {
                if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0)
                    setPixColorXY(x, y, getPixColorXY(x, y - 1));
                else
                    setPixColorXY(x, y, 0);
            }
            else if (SPARKLES)
            {
                if (getPixColorXY(x, y - 1) > 0)
                    setPixColorXY(x, y, getPixColorXY(x, y - 1));
                else
                    setPixColorXY(x, y, 0);
            }
        }
    }

    for (unsigned char x = 0; x < WIDTH; x++)
    {
        uint8_t newX = x;
        if (x > 15)
            newX = x - 15;
        CRGB color = CHSV(
            modes[1].scale * 2.5 + pgm_read_byte(&(hueMask[0][newX])),                     // H
            255,                                                                           // S
            (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
        );
        leds[getPixIndex(newX, 0)] = color;
    }
}
void fireRoutine()
{
    if (effectSlowStart)
    {
        effectSlowStart = false;
        FOR_U8_I(0, 8)
        {
            FOR_U8_J(0, 16)
            {
                matrixValue[i][j] = '0';
            }
        }
        generateLine();
    }
    if (pcnt >= 100)
    {
        shiftUp();
        generateLine();
        pcnt = 0;
    }
    drawFrame(pcnt);
    pcnt += 30;
}

byte hue;
//================= Rainbow V =================// 2
void rainbowVertical()
{
    if (effectSlowStart)
        effectSlowStart = false;
    hue += 2;
    FOR_U8_I(0, HEIGHT)
    {
        CHSV thisColor = CHSV((byte)(hue + i * modes[currentEffectID].scale), 255, 255);
        FOR_U8_J(0, WIDTH)
        {
            setPixColorXY(j, i, thisColor);
        }
    }
}

//================= Rainbow H =================// 3
void rainbowHorizontal()
{
    if (effectSlowStart)
        effectSlowStart = false;
    hue += 2;
    FOR_U8_I(0, WIDTH)
    {
        CHSV thisColor = CHSV((byte)(hue + i * modes[currentEffectID].scale), 255, 255);
        FOR_U8_J(0, HEIGHT)
        {
            setPixColorXY(i, j, thisColor);
        }
    }
}