#pragma once

#include <Arduino.h>

class Timer
{
public:
  Timer(uint32_t interval = 1)
  {
    setInterval(interval);
    _timer = millis();
  }

  void setInterval(uint32_t interval)
  {
    _interval = (interval < 10) ? 10 : interval;
  }

  bool isReady()
  {
    uint32_t now = millis();
    if (now - _timer >= _interval)
    {
      _timer = now;
      return true;
    }
    return false;
  }

  void reset()
  {
    _timer = millis();
  }

private:
  uint32_t _timer = 0;
  uint32_t _interval = 1;
};
