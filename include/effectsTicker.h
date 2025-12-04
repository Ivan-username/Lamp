#pragma once

#include "config.h"

void changeEffect(int id)
{
    DEBUG("Effect switched to:");
    DEBUGLN(id);
    if (id < 0 || id > EFFECTS_AMOUNT - 1)
        return;
    currentEffectID = id;

    effects[currentEffectID]->reset();
    FastLED.clear();
}

// main effects tick function
uint32_t effTimer;
void effectsTick()
{

    if (!isOn)
    {
        FastLED.clear();
    }
    else
    {
        if (millis() - effTimer >= effects[currentEffectID]->_speed)
        {
            effTimer = millis();

            effects[currentEffectID]->routine();

            FastLED.setBrightness(effects[currentEffectID]->_brightness);
            FastLED.show();
        }
    }
}