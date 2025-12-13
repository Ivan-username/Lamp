#pragma once
#include "EventQueue.h"
#include "Button.h"
#include "Timer.h"

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
    if (c)
    {
      evQ.post(Event::evInt16(EventType::BUTTON_CLICK, c));
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
            EventType::BUTTON_HOLD,
            brightnessUp ? 5 : -5));
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