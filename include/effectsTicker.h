#pragma once

#include "config.h"
#include "lampEffects.h"

void changeEffect(int id)
{
    DEBUG("Effect switched to:");
    DEBUGLN(id);
    if (id < 0 || id > EFFECTS_AMOUNT - 1)
        return;
    currentEffectID = id;

    effectSlowStart = true;
    FastLED.clear();
}

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
        if (millis() - effTimer >= modes[currentEffectID].speed)
        {
            effTimer = millis();

            switch (currentEffectID)
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
            FastLED.setBrightness(modes[currentEffectID].brightness);
            FastLED.show();
        }
    }
}