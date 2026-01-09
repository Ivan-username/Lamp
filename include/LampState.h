#pragma once

#include <FileData.h>
#include <LittleFS.h>

#include "EffectData.h"

#include "config.h"

constexpr size_t WIFI_SSID_LEN = 32;
constexpr size_t WIFI_PASS_LEN = 64;

enum class LampWiFiMode : uint8_t
{
  STA = 0,
  AP = 1
};

struct LampState
{
public:
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
    if (wifiMode == LampWiFiMode::AP)
    {
      result += F("AP(");
      result += String(ssidAP);
      result += F(",");
      result += String(passAP);
    }
    else
    {
      result += F("STA(");
      result += String(ssidSTA);
      result += F(",");
      result += String(passSTA);
    }

    result += F(", effect=");
    result += static_cast<uint8_t>(effId);

    return result;
  }

  /* ==================== DATA ==================== */

  bool connected;
  bool power;

  LampWiFiMode wifiMode;

  char ssidSTA[WIFI_SSID_LEN];
  char passSTA[WIFI_PASS_LEN];
  uint8_t localIPSTA[4];

  char ssidAP[WIFI_SSID_LEN];
  char passAP[WIFI_PASS_LEN];
  uint8_t localIPAP[4];
  uint8_t gatewayAP[4];
  uint8_t subnetAP[4];

  EffectId effId;

  /* ==================== API ==================== */
  void setSTASSID(const char *ssid)
  {
    strncpy(ssidSTA, ssid, WIFI_SSID_LEN - 1);
    ssidSTA[WIFI_SSID_LEN - 1] = '\0';
  }

  void setSTAPass(const char *pass)
  {
    strncpy(passSTA, pass, WIFI_PASS_LEN - 1);
    passSTA[WIFI_PASS_LEN - 1] = '\0';
  }

  /* ==================== CTOR ==================== */

  /* ===== CTOR ===== */
  LampState()
  {
    connected = false;
    power = false;

    wifiMode = LampWiFiMode::STA;

    strncpy(ssidSTA, "keenuka", WIFI_SSID_LEN);
    strncpy(passSTA, "ZreTHEA44", WIFI_PASS_LEN);
    localIPSTA[0] = 1;
    localIPSTA[1] = 1;
    localIPSTA[2] = 1;
    localIPSTA[3] = 1;

    strncpy(ssidAP, "JLamp", WIFI_SSID_LEN);
    strncpy(passAP, "12345678", WIFI_PASS_LEN);
    localIPAP[0] = 192;
    localIPAP[1] = 168;
    localIPAP[2] = 4;
    localIPAP[3] = 1;
    gatewayAP[0] = 192;
    gatewayAP[1] = 168;
    gatewayAP[2] = 4;
    gatewayAP[3] = 1;
    subnetAP[0] = 255;
    subnetAP[1] = 255;
    subnetAP[2] = 255;
    subnetAP[3] = 0;

    effId = EffectId::JUST_LAMP;
  }
};

LampState lampState;

FileData lampStateFD(
    &LittleFS,
    "/lamp_state.bin",
    'L',
    &lampState,
    sizeof(lampState),
    10000);