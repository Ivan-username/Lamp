
#pragma once
#include "EffectsController.h"
#include "WebSocketController.h"
#include "HttpController.h"
#include "WiFiController.h"
#include "EventQueue.h"
#include "LampState.h"
#include "config.h"

#include "DEBUG.h"

class LampCore
{
public:
  LampCore(EventQueue &eventQueue,
           EffectsController &effectsController,
           WebSocketController &webSocketController,
           HttpController &httpController,
           WiFiController &wiFiController)
      : evQ(eventQueue),
        effCtrl(effectsController),
        wsCtrl(webSocketController),
        httpCtrl(httpController),
        wifiCtrl(wiFiController) {}

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

    case EventType::POWER_CHANGE:
      lampState.power = !lampState.power;
      break;

    case EventType::EFF_CHANGE:
      lampState.effIndex = e.int16Param;
      break;

    case EventType::EFF_SET_BRIGHTNESS:
      effSets[lampState.effIndex].brightness = e.int16Param;
      break;

    case EventType::EFF_SET_SCALE:
      effSets[lampState.effIndex].scale = e.int16Param;
      break;

    case EventType::EFF_SET_SPEED:
      effSets[lampState.effIndex].speed = e.int16Param;
      break;

    case EventType::WIFI_UPDATE:
      wsCtrl.drop();
      httpCtrl.drop();
      wifiCtrl.setWiFiMode(lampState.wifiMode);
      httpCtrl.init();
      wsCtrl.init();
      break;

    case EventType::HTTP_REQUEST:
      break;

    default:
      break;
    }

    effCtrl.update();
    wsCtrl.update();

#ifdef USE_EEPROM
    lampStateFD.update();
    effSetsFD.update();
#endif

    DEBUGLN(lampState.toString());
  }

private:
  EventQueue &evQ;
  EffectsController &effCtrl;
  WebSocketController &wsCtrl;
  HttpController &httpCtrl;
  WiFiController &wifiCtrl;
};