#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "Timer.h"
#include "Effect.h"
#include "LampState.h"

class EffectsController
{
public:
    EffectsController(Effect *effects[])
        : effs(effects)
    {
    }

    void tick()
    {

        if (effTimer.isReady() && lampState.power)
        {
            effs[lampState.effIndex]->routine(map(effSets[lampState.effIndex].scale, 0, 255, 1, 70));
            FastLED.show();
            isClear = false;
        }
        if (!isClear && !lampState.power)
        {
            FastLED.clear(true);
            isClear = true;
            effs[lampState.effIndex]->reset();
        }
    }

    void init()
    {
        effTimer.setInterval(map(effSets[lampState.effIndex].speed, 0, 255, 10, 100));
        FastLED.setBrightness(map(effSets[lampState.effIndex].brightness, 0, 255, 1, 255));
        effs[lampState.effIndex]->reset();

        animTimer.setInterval(33);
    }

    void update()
    {
        effTimer.setInterval(map(effSets[lampState.effIndex].speed, 0, 255, 10, 100));
        FastLED.setBrightness(map(effSets[lampState.effIndex].brightness, 0, 255, 1, 255));
        effs[lampState.effIndex]->reset();
    }

private:
    bool isClear = false;

    Effect **effs;
    Timer effTimer;
    Timer animTimer;
};