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

    // ============ Power Control ============
    void switchPower()
    {
        _power = !_power;
        if (!_power)
            FastLED.clear(true);
        else
            applyEffect();
    }
    bool getPower() const { return _power; }

    // ============ Effect Control ============
    void applyEffect()
    {
        FastLED.clear(true);
        setBrightness(_effects[_current]->data.brightness);
        updateSpeedTimer(_effects[_current]->data.speed);
    }
    void applyEffect(uint8_t index)
    {
        if (index >= _amount)
            return;
        _current = index;
        FastLED.clear(true);

        setBrightness(_effects[index]->data.brightness);
        updateSpeedTimer(_effects[index]->data.speed);
    }

    uint8_t getCurrentEffectIndex()
    {
        return _current;
    }

    void setPrevEffect()
    {
        applyEffect((_current + _amount - 1) % _amount);
    }
    void setNextEffect()
    {
        applyEffect((_current + 1) % _amount);
    }

    // ============ Effect Parameters Control ============
    void setBrightness(uint8_t b)
    {
        _effects[_current]->setBrightness(b);
        FastLED.setBrightness(b);
    }
    void changeBrightness(int8_t delta)
    {
        uint8_t newBr = constrain(
            (int16_t)_effects[_current]->data.brightness + delta,
            1,
            255);
        setBrightness(newBr);
    }

    void setSpeed(uint8_t s)
    {
        _effects[_current]->setSpeed(s);
        updateSpeedTimer(s);
    }

    void setScale(uint8_t sc) { _effects[_current]->setScale(sc); }

    // ============ Get Current Effect Data ============
    Effect::EffectData getCurrentEffectData()
    {
        return _effects[_current]->data;
    }

private:
    // ============ Internal Helpers ============

    void updateSpeedTimer(uint32_t interval)
    {
        speedTimer.setInterval(interval);
        speedTimer.force();
    }

    bool _power = false;

    uint8_t _amount;
    uint8_t _current = 0;

    Effect **_effects;
    StepTimer speedTimer;
};