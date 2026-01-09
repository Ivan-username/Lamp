#pragma once

#include "EffectManager.h"
#include "WebSocketController.h"
#include "HttpController.h"
#include "WiFiController.h"
#include "EventQueue.h"
#include "LampState.h"
#include "EffectData.h"
#include "LedViewer.h"
#include "config.h"

#include "DEBUG.h"

class LampCore
{
public:
  LampCore(EventQueue &eventQueue,
           EffectManager &effectManager,
           WebSocketController &webSocketController,
           HttpController &httpController,
           WiFiController &wifiController,
           LedViewer &ledViewer)
      : evQ(eventQueue),
        effManager(effectManager),
        wsCtrl(webSocketController),
        httpCtrl(httpController),
        wifiCtrl(wifiController),
        ledViewer(ledViewer) {}

  void tick()
  {
    Event e;
    for (uint8_t i = 0; i < CORE_MESSAGES_PER_TICK; i++)
      if (evQ.poll(e))
        handleEvent(e);
      else
        break;
  }

private:
  void handleEvent(const Event &e)
  {
    switch (e.type)
    {

      // ============ EffSets Events ============
    case EventType::EFF_SET_BRIGHTNESS:
      effSets[static_cast<uint8_t>(lampState.effId)].brightness = constrain(e.int16Param, 1, 255);
      effManager.applyEffectSets();
      updateEffSetsFD();
      break;

    case EventType::EFF_SET_SCALE:
      effSets[static_cast<uint8_t>(lampState.effId)].scale = constrain(e.int16Param, 1, 255);
      effManager.applyEffectSets();
      updateEffSetsFD();
      break;

    case EventType::EFF_SET_SPEED:
      effSets[static_cast<uint8_t>(lampState.effId)].speed = constrain(e.int16Param, 10, 255);
      effManager.applyEffectSets();
      updateEffSetsFD();
      break;

      // ============ LampState Events ============
    case EventType::POWER_CHANGE:
      lampState.power = !lampState.power;
      if (!lampState.power)
        ledViewer.clear();
      updateLampStateFD();
      break;

    case EventType::EFF_CHANGE:
      lampState.effId = static_cast<EffectId>(constrain(e.int16Param, 0, static_cast<uint8_t>(EffectId::COUNT) - 1));
      effManager.setEffect();
      updateLampStateFD();
      break;

    case EventType::WIFI_UPDATE:
      wsCtrl.drop();
      httpCtrl.drop();
      wifiCtrl.setWiFiMode();
      httpCtrl.init();
      wsCtrl.init();
      updateLampStateFD();
      // ============ Other Events ============
    default:
      DEBUG("Unhandled event type: ");
      DEBUGLN(static_cast<uint8_t>(e.type));
      break;
    }

    wsCtrl.broadcast();

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

  EventQueue &evQ;

  EffectManager &effManager;
  WebSocketController &wsCtrl;
  HttpController &httpCtrl;
  WiFiController &wifiCtrl;
  LedViewer &ledViewer;
};