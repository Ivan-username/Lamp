
#pragma once
#include "EffectsController.h"
#include "EventQueue.h"
#include "config.h"

class LampCore
{
public:
  LampCore(EventQueue &eventQueue)
      : evQ(eventQueue) {}

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
      // handle initialization
      break;

    case EventType::WIFI_SWITCH_TO_AP:
      // handle switching to AP mode
      break;

    default:
      break;
    }
  }

private:
  EventQueue &evQ;
};