#pragma once

#include "config.h"

#pragma once

class StepTimer
{
public:
  StepTimer() = default;

  StepTimer(uint32_t interval, bool active = false)
      : _interval(clampInterval(interval))
  {
    uint32_t now = millis();
    _tmr = now;
    _active = active;
  }

  inline void setInterval(uint32_t interval)
  {
    _interval = clampInterval(interval);
  }

  inline bool isReady()
  {
    if (!_active)
      return false;

    uint32_t now = millis();
    if (now - _tmr >= _interval)
    {
      _tmr = now;
      return true;
    }
    return false;
  }

  inline bool runningStop()
  {
    if (expired())
      stop();
    return _active;
  }

  inline void force()
  {
    _tmr = millis() - _interval;
  }

  inline void reset()
  {
    _tmr = millis();
  }

  inline void restart()
  {
    _tmr = millis();
    _active = true;
  }

  inline void stop() { _active = false; }
  inline bool running() const { return _active; }

  inline uint8_t getLength8() const
  {
    if (!_active)
      return 0;

    uint32_t dt = millis() - _tmr;
    if (dt > _interval)
      dt = _interval;

    return (uint32_t(dt) * 255ul) / _interval;
  }

private:
  inline uint32_t clampInterval(uint32_t v) const { return v == 0 ? 1 : v; }

  inline bool expired() const
  {
    return _active && (millis() - _tmr >= _interval);
  }

  uint32_t _tmr = 0;
  uint32_t _interval = 1;
  bool _active = false;
};
