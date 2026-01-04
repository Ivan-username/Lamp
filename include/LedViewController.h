#pragma once

#include <FastLED.h>

#include "Timer.h"
#include "Effect.h"
#include "Animation.h"
#include "LampState.h"

class LedViewController
{
public:
    LedViewController(Effect *effects[])
        : effs(effects)
    {
    }

    void tick()
    {
        if (!timer.isReady() || !lampState.power)
            return;

        effs[lampState.effIndex]->routine(map(effSets[lampState.effIndex].scale, 0, 255, 1, 70));
        FastLED.show();
    }

    void init()
    {
        setEffect();
    }

    void setEffect()
    {
        effs[lampState.effIndex]->reset();
        setEffectSets();
    }

    void setEffectSets()
    {
        timer.setInterval(map(effSets[lampState.effIndex].speed, 0, 255, 10, 100));
        FastLED.setBrightness(map(effSets[lampState.effIndex].brightness, 0, 255, 1, 255));
    }

    void clear()
    {
        FastLED.clear(true);
    }

private:
    Effect **effs;
    Timer timer;
};