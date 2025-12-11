
#pragma once
#include "EffectsController.h"
#include "EventQueue.h"
#include "config.h"

class LampCore
{
public:
  LampCore(EventQueue &eventQueue, EffectsController &effectsController)
      : evQ(eventQueue), effCtrl(effectsController) {}

  void init()
  {
    evQ.post(Event::ev(EventType::INIT));
  }

  void tick()
  {
    Event e;
    evQ.poll(e);
    handleEvent(e);
  }

  void handleEvent(const Event &e)
  {
    switch (e.type)
    {
    case EventType::INIT:
      effCtrl.applyEffect();
      break;

    case EventType::BUTTON_CLICK:
      if (e.int16Param == 1)
        effCtrl.switchPower();
      else if (e.int16Param == 2)
        effCtrl.setNextEffect();
      else if (e.int16Param == 3)
        effCtrl.setPrevEffect();
      break;

    case EventType::BUTTON_HOLD:
      effCtrl.changeBrightness(e.int16Param);
      break;

    default:
      break;
    }
  }

private:
  EventQueue &evQ;
  EffectsController &effCtrl;
};