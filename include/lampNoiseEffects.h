
#pragma once

#include "config.h"
#include "lampuLedUtilits.h"

#define MAX_DIMENSION (max(WIDTH, HEIGHT))
#if (WIDTH > HEIGHT)
uint8_t noise[WIDTH][WIDTH];
#else
uint8_t noise[HEIGHT][HEIGHT];
#endif

CRGBPalette16 currentPalette(PartyColors_p);
uint8_t colorLoop = 1;
uint8_t ihue = 0;

uint16_t speed = 20; // speed is set dynamically once we've started up
uint16_t scale = 30; // scale is set dynamically once we've started up

// ******************* СЛУЖЕБНЫЕ *******************
void fillNoiseLED()
{
    uint8_t dataSmoothing = 0;
    if (speed < 50)
    {
        dataSmoothing = 200 - (speed * 4);
    }
    for (int i = 0; i < MAX_DIMENSION; i++)
    {
        int ioffset = scale * i;
        for (int j = 0; j < MAX_DIMENSION; j++)
        {
            int joffset = scale * j;

            uint8_t data = inoise8(x + ioffset, y + joffset, z);

            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));

            if (dataSmoothing)
            {
                uint8_t olddata = noise[i][j];
                uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
                data = newdata;
            }

            noise[i][j] = data;
        }
    }
    z += speed;

    // apply slow drift to X and Y, just for visual variation.
    x += speed / 8;
    y -= speed / 16;

    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            uint8_t index = noise[j][i];
            uint8_t bri = noise[i][j];
            // if this palette is a 'loop', add a slowly-changing base value
            if (colorLoop)
            {
                index += ihue;
            }
            // brighten up, as the color palette itself often contains the
            // light/dark dynamic range desired
            if (bri > 127)
            {
                bri = 255;
            }
            else
            {
                bri = dim8_raw(bri * 2);
            }
            CRGB color = ColorFromPalette(currentPalette, index, bri);
            drawPixelXY(i, j, color); // leds[getPixelNumber(i, j)] = color;
        }
    }
    ihue += 1;
}

void fillnoise8()
{
    for (int i = 0; i < MAX_DIMENSION; i++)
    {
        int ioffset = scale * i;
        for (int j = 0; j < MAX_DIMENSION; j++)
        {
            int joffset = scale * j;
            noise[i][j] = inoise8(x + ioffset, y + joffset, z);
        }
    }
    z += speed;
}

void lavaNoise()
{
    if (effectSlowStart)
    {
        effectSlowStart = false;
        currentPalette = LavaColors_p;
        scale = modes[currentModeID].scale;
        speed = modes[currentModeID].speed;
        colorLoop = 0;
    }
    fillNoiseLED();
}