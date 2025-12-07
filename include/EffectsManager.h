#pragma once

#include <Arduino.h>
#include <FastLED.h>

class EffectsManager
{
public:
    EffectsManager(uint8_t amount, Effect **effects)
        : _amount(amount), _effects(effects) {}

    void tick()
    {
        // add speed timer
        _effects[_current]->_speed;
        //

        if (_amount == 0)
            return;
        _effects[_current]->routine();
        FastLED.show();
    }

    void applyState(uint8_t eff, uint8_t br, uint8_t sp, uint8_t sc)
    {
        setEffect(eff);
        _effects[_current]->setBrightness(br);
        _effects[_current]->setSpeed(sp);
        _effects[_current]->setScale(sc);
    }

    void setEffect(uint8_t id)
    {
        if (id >= _amount)
            return;
        _current = id;
        // _effects[_current]->reset();
        FastLED.clear(true);
    }

    void setBrightness(uint8_t b)
    {
        _effects[_current]->setBrightness(b);
        FastLED.setBrightness(b);
    }
    void setSpeed(uint8_t s) { _effects[_current]->setSpeed(s); }
    void setScale(uint8_t sc) { _effects[_current]->setScale(sc); }

private:
    uint8_t _amount;
    uint8_t _current = 0;

    Effect **_effects;
};