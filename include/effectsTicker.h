#pragma once

#include "config.h"
#include "lampEffects.h"
#include "lampNoiseEffects.h"

// main effects tick function

uint32_t effTimer;
void effectsTick()
{
    if (millis() - effTimer >= modes[currentModeID].speed)
    {
        effTimer = millis();

        switch (currentModeID)
        {
        case 0:
            offRoutine();
            break;
        case 1:
            fireRoutine();
            break;
        case 2:
            lavaNoise();
            break;
        default:
            break;
        }

        FastLED.setBrightness(modes[currentModeID].brightness);
        FastLED.show();
    }
}