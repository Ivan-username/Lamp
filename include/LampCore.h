
#pragma once
#include "EffectsManager.h"
#include "EventBus.h"

#include "config.h"

class LampCore
{
public:
  LampCore(EventBus &bus, EffectsManager &effects)
      : bus(bus), effectsManager(effects) {}

  void init()
  {
    bus.subscribe([this](const Event &ev)
                  { handleEvent(ev); });
    bus.publish({Event::Init});
  }

  // const LampState &getState() const { return state; }

private:
  EventBus &bus;
  EffectsManager &effectsManager;
  Effect::LampData data;
  uint8_t currentEffect = 0;

  void handleEvent(const Event &ev)
  {

    if (ev.type == Event::StateChanged)
      return;

    effectsManager.getCurrentEffect(currentEffect);
    effectsManager.getCurrentEffectData(data);

    switch (ev.type)
    {
    case Event::Init:

      effectsManager.applyEffect();
      break;

    case Event::TogglePower:
      effectsManager.switchPower();
      DEBUGLN(String("Power ") + (effectsManager.getPower() ? "ON" : "OFF"));
      break;

    case Event::SetEffect:
      if (ev.strValue == "+")
        currentEffect = (currentEffect + ev.intValue) % EFFECTS_AMOUNT;
      else if (ev.strValue == "-")
        currentEffect = (currentEffect == 0
                             ? EFFECTS_AMOUNT - 1
                             : currentEffect - ev.intValue);
      else
        currentEffect = constrain(ev.intValue, 0, EFFECTS_AMOUNT - 1);

      effectsManager.setEffect(currentEffect);
      DEBUGLN("Set effect to " + String(currentEffect));
      break;

    case Event::SetBrightness:
      if (ev.strValue == "+")
        data.brightness = constrain(data.brightness + ev.intValue, 1, 255);
      else if (ev.strValue == "-")
        data.brightness = constrain(data.brightness - ev.intValue, 1, 255);
      else
        data.brightness = constrain(ev.intValue, 1, 255);

      effectsManager.setBrightness(data.brightness);
      DEBUGLN("Set brightness to " + String(data.brightness));
      break;

    case Event::SetSpeed:
      data.speed = constrain(ev.intValue, 1, 255);
      effectsManager.setSpeed(data.speed);
      DEBUGLN("Set speed to " + String(data.speed));
      break;

    case Event::SetScale:
      data.scale = constrain(ev.intValue, 1, 255);
      effectsManager.setScale(data.scale);
      DEBUGLN("Set scale to " + String(data.scale));
      break;

    default:
      break;
    }

    // уведомляем наблюдателей (WebSocket и др.)
    bus.publish({Event::StateChanged});
  }
};