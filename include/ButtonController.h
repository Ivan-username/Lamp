#pragma once
#include "EventBus.h"
#include "StepTimer.h"

class ButtonController
{
public:
  ButtonController(Button &button, EventBus &bus)
      : btn(button), bus(bus), holdTimer(50, false) {}

  void tick()
  {
    btn.tick();

    uint8_t c = btn.getClicks();
    if (c == 1)
      bus.publish({Event::TogglePower});
    if (c == 2)
      bus.publish({Event::NextEffect});
    if (c == 3)
      bus.publish({Event::PrevEffect});

    if (btn.isHold())
    {
      if (holdTimer.isReady())
      {
        bus.publish({Event::SetBrightness, 5, brDirection ? "+" : "-"});
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
  Button &btn;
  EventBus &bus;
  StepTimer holdTimer;
};