#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "StepTimer.h"
#include "Effect.h"

class EffectsController
{
public:
    EffectsController(uint8_t amount, Effect **effects)
        : _amount(amount), _effects(effects), speedTimer(50, true) {}

    void tick()
    {
        if (speedTimer.isReady() && _power)
        {
            if (_amount == 0)
                return;
            _effects[_current]->routine();
            FastLED.show();
            speedTimer.restart();
        }
    }

    void switchPower()
    {
        _power = !_power;
        if (!_power)
            FastLED.clear(true);
    }
    bool getPower() const { return _power; }

    void applyEffect()
    {
        setEffect(_current);
        updateSpeedTimer(_effects[_current]->data.speed);
    }

    void setEffect(uint8_t id)
    {
        if (id >= _amount)
            return;
        _current = id;
        FastLED.clear(true);
        updateSpeedTimer(_effects[_current]->data.speed);
    }

    void setBrightness(uint8_t b)
    {
        _effects[_current]->setBrightness(b);
        FastLED.setBrightness(b);
    }

    void setSpeed(uint8_t s)
    {
        _effects[_current]->setSpeed(s);
        updateSpeedTimer(s);
    }

    void setScale(uint8_t sc) { _effects[_current]->setScale(sc); }

    void updateSpeedTimer(uint32_t interval)
    {
        speedTimer.setInterval(interval);
        speedTimer.force();
    }

    void getCurrentEffectData(Effect::EffectData &data)
    {
        data = _effects[_current]->data;
    }
    void getCurrentEffect(uint8_t &id)
    {
        id = _current;
    }

private:
    bool _power = false;

    uint8_t _amount;
    uint8_t _current = 0;

    Effect **_effects;
    StepTimer speedTimer;
};