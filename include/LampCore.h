
#pragma once
#include "EffectsController.h"
#include "WebSocketController.h"
#include "EventQueue.h"
#include "LampState.h"
#include "config.h"

class LampCore
{
public:
  LampCore(EventQueue &eventQueue,
           EffectsController &effectsController,
           WebSocketController &webSocketController)
      : evQ(eventQueue),
        effCtrl(effectsController),
        wsCtrl(webSocketController) {}

  void init() {}

  void tick()
  {
    Event e;
    if (evQ.poll(e))
      handleEvent(e);
  }

  void handleEvent(const Event &e)
  {
    switch (e.type)
    {
      // ============ Button Events ============
    case EventType::BUTTON_CLICK:
      if (e.int16Param == 1)
        lampState.power = !lampState.power;
      if (e.int16Param == 2)
        lampState.effIndex = (lampState.effIndex + 1) % lampState.effAmount;
      else if (e.int16Param == 3)
        lampState.effIndex = (lampState.effIndex + lampState.effAmount - 1) % lampState.effAmount;
      break;

    case EventType::BUTTON_HOLD:
      effSets[lampState.effIndex].brightness = constrain(effSets[lampState.effIndex].brightness + e.int16Param, 1, 255);
      break;

      // ============ WS / HTTP Events ============
    case EventType::WS_MESSAGE:
      if (e.stringParam.startsWith("POWER"))
        lampState.power = !lampState.power;
      else if (e.stringParam.startsWith("EFFECT:"))
        lampState.effIndex = constrain(e.stringParam.substring(7).toInt(), 0, lampState.effAmount - 1);
      else if (e.stringParam.startsWith("BRIGHT:"))
        effSets[lampState.effIndex].brightness = e.stringParam.substring(7).toInt();
      else if (e.stringParam.startsWith("SCALE:"))
        effSets[lampState.effIndex].scale = e.stringParam.substring(6).toInt();
      else if (e.stringParam.startsWith("SPEED:"))
        effSets[lampState.effIndex].speed = e.stringParam.substring(6).toInt();
      break;

    case EventType::HTTP_REQUEST:
      break;

    default:
      break;
    }

    effCtrl.update();
    wsCtrl.update();
  }

private:
  EventQueue &evQ;
  EffectsController &effCtrl;
  WebSocketController &wsCtrl;
};