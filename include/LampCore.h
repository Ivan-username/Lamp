#pragma once

#include "LedViewController.h"
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
           LedViewController &ledViewController,
           WebSocketController &webSocketController,
           HttpController &httpController,
           WiFiController &wifiController)
      : evQ(eventQueue),
        ledViewCtrl(ledViewController),
        wsCtrl(webSocketController),
        httpCtrl(httpController),
        wifiCtrl(wifiController) {}

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

      // ============ EffSets Events ============
    case EventType::EFF_SET_BRIGHTNESS:
      effSets[lampState.effIndex].brightness = e.int16Param;
      ledViewCtrl.setEffectSets();
      updateEffSetsFD();
      break;

    case EventType::EFF_SET_SCALE:
      effSets[lampState.effIndex].scale = e.int16Param;
      ledViewCtrl.setEffectSets();
      updateEffSetsFD();
      break;

    case EventType::EFF_SET_SPEED:
      effSets[lampState.effIndex].speed = e.int16Param;
      ledViewCtrl.setEffectSets();
      updateEffSetsFD();
      break;

      // ============ LampState Events ============
    case EventType::POWER_CHANGE:
      lampState.power = !lampState.power;
      if (lampState.power == false)
        ledViewCtrl.clear();
      updateLampStateFD();
      break;

    case EventType::EFF_CHANGE:
      lampState.effIndex = e.int16Param;
      ledViewCtrl.setEffect();
      updateLampStateFD();
      break;

    case EventType::WIFI_UPDATE:
      wsCtrl.drop();
      httpCtrl.drop();
      wifiCtrl.setWiFiMode(lampState.wifiMode);
      httpCtrl.init();
      wsCtrl.init();
      updateLampStateFD();
      break;

    default:
      DEBUGLN("Unhandled event type: " + String(static_cast<uint8_t>(e.type)));
      break;
    }

    wsCtrl.update();

    DEBUGLN(lampState.toString());
  }

#ifdef USE_EEPROM
  void updateLampStateFD()
  {
    lampStateFD.update();
  }
  void updateEffSetsFD()
  {
    effSetsFD.update();
  }
#else
  void updateLampStateFD() { return; }
  void updateEffSetsFD() { return; }
#endif

private:
  EventQueue &evQ;
  LedViewController &ledViewCtrl;
  WebSocketController &wsCtrl;
  HttpController &httpCtrl;
  WiFiController &wifiCtrl;
};