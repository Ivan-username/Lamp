#pragma once
#include "EventQueue.h"
#include "Button.h"
#include "Timer.h"
#include "LampState.h"

#include "DEBUG.h"

class ButtonController
{
public:
  ButtonController(EventQueue &eventQueue, uint8_t pin = D2, bool pullup = true)
      : evQ(eventQueue), btn(pin, pullup), holdTimer(50) {}

  void tick()
  {
    btn.tick();

    handleClicks();
    handleHold();
  }

private:
  EventQueue &evQ;
  Button btn;
  Timer holdTimer;

  bool brightnessUp = true;   // направление изменения
  bool holdProcessed = false; // защита от повторного переключения

  void handleClicks()
  {
    uint8_t c = btn.getClicks();
    switch (c)
    {
    case 1:
      evQ.post(Event::ev(EventType::POWER_CHANGE));
      break;
    case 2:
      evQ.post(Event::evInt16(EventType::EFF_CHANGE, (lampState.effIndex + 1) % lampState.effAmount));
      break;
    case 3:
      evQ.post(Event::evInt16(EventType::EFF_CHANGE, (lampState.effIndex + lampState.effAmount - 1) % lampState.effAmount));
      break;
    default:
      DEBUGLN("ButtonController: unhandled clicks count: " + String(c));
      break;
    }
  }

  void handleHold()
  {
    if (btn.isHold())
    {
      // первое вхождение в удержание
      if (!holdProcessed)
      {
        brightnessUp = !brightnessUp; // меняем направление
        holdProcessed = true;
        holdTimer.reset();
      }

      // периодическое изменение яркости
      if (holdTimer.isReady())
      {
        evQ.post(Event::evInt16(
            EventType::EFF_SET_BRIGHTNESS,
            brightnessUp ? constrain(effSets[lampState.effIndex].brightness + 5, 1, 255) : constrain(effSets[lampState.effIndex].brightness - 5, 1, 255)));
      }
    }
    else
    {
      // кнопка отпущена → готовимся к следующему удержанию
      holdProcessed = false;
      holdTimer.reset();
    }
  }
};