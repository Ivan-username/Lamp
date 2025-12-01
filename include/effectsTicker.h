#pragma once

#include "config.h"
#include "lampEffects.h"

// main effects tick function

uint32_t effTimer;
void effectsTick()
{

    if (!isOn)
    {
        offRoutine();
        FastLED.show();
    }
    else
    {
        if (effectSlowStart)
        {
            FastLED.clear();
        }
        if (millis() - effTimer >= modes[currentModeID].speed)
        {
            effTimer = millis();

            switch (currentModeID)
            {
            case 0:
            {
                lampRoutine();
                break;
            }
            case 1:
            {
                fireRoutine();
                break;
            }
            case 2:
            {
                rainbowVertical();
                break;
            }
            case 3:
            {
                rainbowHorizontal();
                break;
            }
            }
            FastLED.setBrightness(modes[currentModeID].brightness);
            FastLED.show();
        }
    }
}