#pragma once

#include "config.h"
#include <Arduino.h>

#pragma once

enum class LampWiFiMode : uint8_t
{
  STA = 0,
  AP = 1
};

struct LampState
{
public:
  /* ==================== API ==================== */

  void setSTASSID(String ssid)
  {
    ssidSTA = ssid;
    changed = true;
  }

  void setSTAPass(String pass)
  {
    passSTA = pass;
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
    result += connected ? F("CONNECTED") : F("DISCONNECTED");

    result += F(", wifiMode=");
    result += result += (wifiMode == LampWiFiMode::AP) ? F("AP") : F("STA");

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
  bool connected;
  LampWiFiMode wifiMode;

  String ssidSTA;
  String passSTA;
  IPAddress localIPSTA;

  String ssidAP;
  String passAP;
  IPAddress localIPAP;
  IPAddress gatewayAP;
  IPAddress subnetAP;

  bool power;
  uint8_t effAmount;
  uint8_t effIndex;

  /* ==================== CTOR ==================== */

  LampState()
      : changed(true),
        connected(false),
        wifiMode(LampWiFiMode::STA),
        ssidSTA("keenetic"),
        passSTA("12345678"),
        localIPSTA(1, 1, 1, 1),
        ssidAP("JLamp"),
        passAP("12345678"),
        localIPAP(192, 168, 4, 1),
        gatewayAP(192, 168, 4, 1),
        subnetAP(255, 255, 255, 0),
        power(false),
        effAmount(EFFECTS_AMOUNT),
        effIndex(0)
  {
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
