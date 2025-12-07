
#pragma once
#include "LampState.h"
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
  }

  const LampState &getState() const { return state; }

private:
  LampState state;
  EventBus &bus;
  EffectsManager &effectsManager;

  void handleEvent(const Event &ev)
  {

    if (ev.type == Event::StateChanged)
      return;

    switch (ev.type)
    {
    case Event::TogglePower:
      state.isOn = !state.isOn;
      if (!state.isOn)
        FastLED.clear(true);
      DEBUGLN(String("Power ") + (state.isOn ? "ON" : "OFF"));
      break;

    case Event::SetEffect:
      state.currentEffect = ev.intValue;
      effectsManager.setEffect(state.currentEffect);
      DEBUGLN("Set effect to " + String(state.currentEffect));
      break;

    case Event::NextEffect:
      state.currentEffect = (state.currentEffect + 1) % EFFECTS_AMOUNT;
      effectsManager.setEffect(state.currentEffect);
      DEBUGLN("Set effect to " + String(state.currentEffect));
      break;

    case Event::PrevEffect:
      state.currentEffect = (state.currentEffect == 0
                                 ? EFFECTS_AMOUNT - 1
                                 : state.currentEffect - 1);
      effectsManager.setEffect(state.currentEffect);
      DEBUGLN("Set effect to " + String(state.currentEffect));
      break;

    case Event::SetBrightness:
      if (ev.strValue == "+")
        state.brightness = constrain(state.brightness + ev.intValue, 1, 255);
      else if (ev.strValue == "-")
        state.brightness = constrain(state.brightness - ev.intValue, 1, 255);
      else
        state.brightness = constrain(ev.intValue, 1, 255);

      effectsManager.setBrightness(state.brightness);
      DEBUGLN("Set brightness to " + String(state.brightness));
      break;

    case Event::SetSpeed:
      state.speed = constrain(ev.intValue, 1, 255);
      effectsManager.setSpeed(state.speed);
      DEBUGLN("Set speed to " + String(state.speed));
      break;

    case Event::SetScale:
      state.scale = constrain(ev.intValue, 1, 255);
      effectsManager.setScale(state.scale);
      DEBUGLN("Set scale to " + String(state.scale));
      break;

    default:
      break;
    }

    // уведомляем наблюдателей (WebSocket и др.)
    bus.publish({Event::StateChanged});
  }
};