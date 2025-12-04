#pragma once

#include "config.h"

#define CLICKS_TOUT 400
#define BTN_HOLD 500
#define BTN_DEBOUNCE 30

class Button
{
public:
  Button(byte pin, bool pullup = true) : _pin(pin), _inv(pullup)
  {
    pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
  }

  void tick()
  {
    uint32_t now = millis();
    bool state = digitalRead(_pin);
    if (_inv)
      state = !state;

    if (state != _lastState)
    {
      if (now - _debounceTimer > BTN_DEBOUNCE)
      {
        _lastState = state;

        if (_lastState)
        {
          _pressTime = now;
          _holdFlag = false;
        }
        else
        {
          if (!_holdFlag)
          {
            _clicksCounter++;
            _clickTimer = now;
          }
        }
      }
      _debounceTimer = now;
    }

    if (_lastState && !_holdFlag && (now - _pressTime >= BTN_HOLD))
    {
      _holdFlag = true;
      _clicksCounter = 0;
    }
  }

  uint8_t getClicks()
  {
    if (_clicksCounter > 0 && (millis() - _clickTimer > CLICKS_TOUT))
    {
      uint8_t temp = _clicksCounter;
      _clicksCounter = 0;
      return temp;
    }
    return 0;
  }

  bool isHold()
  {
    return _lastState && _holdFlag;
  }

private:
  byte _pin;
  bool _inv;

  bool _lastState = false;
  bool _holdFlag = false;

  uint8_t _clicksCounter = 0;

  uint32_t _pressTime = 0;
  uint32_t _clickTimer = 0;
  uint32_t _debounceTimer = 0;
};