#pragma once

#include "Timer.h"
#include "Effect.h"
#include "EffectData.h"
#include "LampState.h"

class EffectManager
{
public:
    EffectManager(Effect *effects[])
        : effs(effects)
    {
    }

    bool view()
    {
        if (effTimer.isReady() && lampState.power)
        {
            currentEffect->routine();
            return true;
        }
        return false;
    }

    void setEffect()
    {
        currentEffect = effs[static_cast<uint8_t>(lampState.effId)];
        currentEffect->reset();
        applyEffectSets();
    }
    void applyEffectSets()
    {
        effTimer.setInterval(effSets[static_cast<uint8_t>(lampState.effId)].speed);
    }

private:
    Effect **effs;
    Effect *currentEffect;
    Timer effTimer;
};