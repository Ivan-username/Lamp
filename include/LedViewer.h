
#pragma once

#include <FastLED.h>

#include "Timer.h"
#include "Effect.h"
#include "EffectManager.h"
#include "Ring.h"

constexpr uint16_t LED_VIEWER_INTERVAL = 10; // ms

class LedViewer
{
public:
  LedViewer(EffectManager &effectManager, Ring &ring) : effManager(effectManager), ring(ring) {};

  void tick()
  {
    if (!offsetTimer.isReady())
      return;

    if (effManager.view() || ring.view())
      FastLED.show();
  }
  void clear()
  {
    FastLED.clear(true);
  }

private:
  EffectManager &effManager;
  Ring &ring;
  Timer offsetTimer{LED_VIEWER_INTERVAL};
};