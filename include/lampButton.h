#pragma once

#include "config.h"
#include "effectsTicker.h"
#include "timerMillis.h"

void buttonTick()
{
  static bool brDir = true;
  static bool isHoldingProcess = false;
  static timerMillis stepTmr(50, false);

  btn.tick();

  uint8_t c = btn.getClicks();
  if (c > 0)
  {
    DEBUG("clicks: ");
    DEBUGLN(c);
    switch (c)
    {
    case 1:
      isOn = !isOn;
      break;

    case 2:
      if (currentEffectID + 1 >= EFFECTS_AMOUNT)
        currentEffectID = 0;
      else
        currentEffectID++;
      changeEffect(currentEffectID);
      break;

    case 3:
      if (currentEffectID - 1 < 0)
        currentEffectID = EFFECTS_AMOUNT - 1;
      else
        currentEffectID--;
      changeEffect(currentEffectID);
      break;
    }
  }

  if (isOn && btn.isHold())
  {
    if (!isHoldingProcess)
    {
      isHoldingProcess = true;
      stepTmr.restart();
    }

    if (stepTmr.isReady())
    {
      int temp = modes[currentEffectID].brightness;

      if (brDir)
        temp += 5;
      else
        temp -= 5;

      temp = constrain(temp, 1, 255);
      modes[currentEffectID].brightness = temp;
    }
  }
  else
  {
    if (isHoldingProcess)
    {
      isHoldingProcess = false;
      brDir = !brDir;
    }
  }
}