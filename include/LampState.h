#pragma once

#include "config.h"
#include <Arduino.h>

#pragma once

constexpr uint8_t SSID_MAX_LEN = 32;
constexpr uint8_t PASS_MAX_LEN = 64;

struct LampState
{
public:
  /* ==================== API ==================== */

  inline void safeCopy(char *dst, size_t dstSize, const char *src)
  {
    if (!dst || dstSize == 0)
      return;

    if (!src)
    {
      dst[0] = '\0';
      return;
    }

    strncpy(dst, src, dstSize - 1);
    dst[dstSize - 1] = '\0';
  }

  void setSSID(const char *ssid)
  {
    if (wifiMode)
      safeCopy(ssidSTA, sizeof(ssidSTA), ssid);
    else
      safeCopy(ssidAP, sizeof(ssidAP), ssid);
    changed = true;
  }

  void setPassword(const char *pass)
  {
    if (wifiMode)
      safeCopy(passSTA, sizeof(passSTA), pass);
    else
      safeCopy(passAP, sizeof(passAP), pass);
    changed = true;
  }

  String toString() const
  {
    String result;
    result.reserve(140);

    result += F("LampState: ");
    result += F("power=");
    result += power ? F("ON") : F("OFF");

    result += F(", wifi=");
    result += wifi ? F("CONNECTED") : F("DISCONNECTED");

    result += F(", wifiMode=");
    result += wifiMode ? F("AP") : F("STA");

    result += F(", effect=");
    result += effIndex;
    result += '/';
    result += (effAmount > 0 ? effAmount - 1 : 0);

    result += F(", changed=");
    result += changed ? F("true") : F("false");

    return result;
  }

  /* ==================== DATA ==================== */

  bool changed;
  bool wifi;
  uint8_t wifiMode;

  char ssidSTA[SSID_MAX_LEN + 1];
  char passSTA[PASS_MAX_LEN + 1];

  char ssidAP[SSID_MAX_LEN + 1];
  char passAP[PASS_MAX_LEN + 1];

  bool power;
  uint8_t effAmount;
  uint8_t effIndex;

  /* ==================== CTOR ==================== */

  LampState()
      : changed(true),
        wifi(true),
        wifiMode(0),
        power(false),
        effAmount(EFFECTS_AMOUNT),
        effIndex(0)
  {
    ssidSTA[0] = '\0';
    passSTA[0] = '\0';
    ssidAP[0] = '\0';
    passAP[0] = '\0';
  }
};

struct EffSets
{
  uint8_t brightness = 30;
  uint8_t scale = 30;
  uint8_t speed = 30;
};

EffSets effSets[EFFECTS_AMOUNT];
LampState lampState;
