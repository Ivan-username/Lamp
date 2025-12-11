#pragma once
#include "EventQueue.h"
#include "StepTimer.h"
#include "Button.h"

class ButtonController
{
public:
  ButtonController(EventQueue &eventQueue, uint8_t pin = D2, bool pullup = true)
      : evQ(eventQueue), btn(pin, pullup), holdTimer(50, false) {}

  void tick()
  {
    btn.tick();

    uint8_t c = btn.getClicks();
    switch (c)
    {
    case 1:
      evQ.post(Event::evInt16(EventType::BUTTON_CLICK, 1));
      break;
    case 2:
      evQ.post(Event::evInt16(EventType::BUTTON_CLICK, 2));
      break;
    case 3:
      evQ.post(Event::evInt16(EventType::BUTTON_CLICK, 3));
      break;
    }

    if (btn.isHold())
    {
      if (holdTimer.isReady())
      {
        evQ.post(Event::evInt16(EventType::BUTTON_HOLD, brDirection ? 5 : -5));
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
  EventQueue &evQ;
  Button btn;
  StepTimer holdTimer;
};