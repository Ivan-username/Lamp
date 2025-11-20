#pragma once

#include "config.h"
#include "lampuLedUtilits.h"

unsigned char matrixValue[8][16];

void ledsSetup()
{
    FastLED.addLeds<WS2812, D4, GRB>(leds, LED_NUM);
    FastLED.setBrightness(modes[0].brightness);
    FastLED.clear(true);
}
//================= Off =================//
void offRoutine()
{
    fillAll(CRGB::Black);
}

//================= Lamp =================// 0
void lampRoutine()
{
    if (effectSlowStart)
    {
        effectSlowStart = false;
    }
    fillAll(CRGB::White);
}
//================= Fire =================// 1
#define SPARKLES 1 // вылетающие угольки вкл выкл
unsigned char line[WIDTH];
int pcnt = 0;
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

// Randomly generate the next line (matrix row)

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

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

void drawFrame(int pcnt)
{
    int nextv;

    // each row interpolates with the one before it
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

                leds[getPixelNumber(x, y)] = color;
            }
            else if (y == 8 && SPARKLES)
            {
                if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0)
                    drawPixelXY(x, y, getPixColorXY(x, y - 1));
                else
                    drawPixelXY(x, y, 0);
            }
            else if (SPARKLES)
            {

                // старая версия для яркости
                if (getPixColorXY(x, y - 1) > 0)
                    drawPixelXY(x, y, getPixColorXY(x, y - 1));
                else
                    drawPixelXY(x, y, 0);
            }
        }
    }

    // first row interpolates with the "next" line
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
        leds[getPixelNumber(newX, 0)] = color;
    }
}

void fireRoutine()
{
    if (effectSlowStart)
    {
        effectSlowStart = false;
        //        FastLED.clear();
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

//================= Lava =================// 2
void lavaNoise()
{
    if (effectSlowStart)
    {
        effectSlowStart = false;
        currentPalette = LavaColors_p;

        colorLoop = 0;
    }

    scale = modes[currentModeID].scale;
    speed = modes[currentModeID].speed;
    fillNoiseLED();
}