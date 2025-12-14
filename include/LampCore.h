
#pragma once
#include "EffectsController.h"
#include "WebSocketController.h"
#include "HttpController.h"
#include "WiFiController.h"
#include "EventQueue.h"
#include "LampState.h"
#include "config.h"

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
      else if (e.stringParam.startsWith("STA:"))
      {
        if (lampState.wifiMode == LampWiFiMode::STA)
          return;

        String payload = e.stringParam.substring(4);
        int8_t commaIndex = payload.indexOf(',');

        if (commaIndex == -1)
          return;

        lampState.ssidSTA = payload.substring(0, commaIndex);
        lampState.passSTA = payload.substring(commaIndex + 1);
        lampState.wifiMode = LampWiFiMode::STA;
        evQ.post(Event::ev(EventType::WIFI_UPDATE));
      }
      else if (e.stringParam.startsWith("AP"))
      {
        if (lampState.wifiMode == LampWiFiMode::AP)
          return;

        lampState.wifiMode = LampWiFiMode::AP;
        evQ.post(Event::ev(EventType::WIFI_UPDATE));
      }
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
  }

private:
  EventQueue &evQ;
  EffectsController &effCtrl;
  WebSocketController &wsCtrl;
  HttpController &httpCtrl;
  WiFiController &wifiCtrl;
};