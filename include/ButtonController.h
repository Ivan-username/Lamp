#pragma once
#include "EventBus.h"
#include "StepTimer.h"
#include "Button.h"

class ButtonController
{
public:
  ButtonController(uint8_t pin = D2, bool pullup)
      : btn(pin, pullup), holdTimer(50, false) {}

  void tick()
  {
    btn.tick();

    uint8_t c = btn.getClicks();
    switch (c)
    {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    }

    if (btn.isHold())
    {
      if (holdTimer.isReady())
      {
      }
      brDirChanged = false;
    }
    else
    {
      if (!brDirChanged)
      {
        brDirection = !brDirection;
        brDirChanged = true;
      }
      holdTimer.restart();
    }
  }

private:
  bool brDirection = true;
  bool brDirChanged = true;
  Button btn;
  StepTimer holdTimer;
};