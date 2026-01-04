#pragma once

#include <ESP8266WiFi.h>

#include "EventQueue.h"
#include "Timer.h"
#include "LampState.h"

class WiFiController
{
public:
    WiFiController(EventQueue &eventQueue) : evQ(eventQueue)
    {
    }

    void tick()
    {
        if (!checkTimer.isReady() || lampState.wifiMode == LampWiFiMode::AP)
            return;

        wl_status_t status = WiFi.status();

        if (status == WL_CONNECTED)
        {
            if (!lampState.connected)
            {
                lampState.connected = true;
                IPAddress ip = WiFi.localIP();
                lampState.localIPSTA[0] = ip[0];
                lampState.localIPSTA[1] = ip[1];
                lampState.localIPSTA[2] = ip[2];
                lampState.localIPSTA[3] = ip[3];

                evQ.post(Event::ev(EventType::WIFI_CONNECTED));
            }
            return;
        }

        // ---- NOT CONNECTED ----

        if (lampState.connected)
        {
            lampState.connected = false;
            evQ.post(Event::ev(EventType::WIFI_DISCONNECTED));
            return;
        }

        // ---- STILL TRYING ----

        if (tries > 0)
        {
            tries--;
            return;
        }

        // ---- FAIL → SWITCH TO AP ----

        tries = MAX_TRIES;
        lampState.wifiMode = LampWiFiMode::AP;
        evQ.post(Event::ev(EventType::WIFI_UPDATE));
    }

    void init()
    {
        evQ.post(Event::ev(EventType::WIFI_UPDATE));
    }

    void setWiFiMode(LampWiFiMode mode)
    {

        // 2. disconnect STA
        WiFi.disconnect(true);
        delay(50);

        // 3. WiFi OFF
        WiFi.mode(WIFI_OFF);
        delay(100);

        if (lampState.wifiMode == LampWiFiMode::STA)
        {
            WiFi.mode(WIFI_STA);
            delay(50);
            WiFi.begin(lampState.ssidSTA, lampState.passSTA);
        }
        else
        {
            WiFi.mode(WIFI_AP);
            delay(50);
            WiFi.softAP(lampState.ssidAP, lampState.passAP);
            WiFi.softAPConfig(
                lampState.localIPAP,
                lampState.gatewayAP,
                lampState.subnetAP);
        }
    }

private:
    static constexpr uint8_t MAX_TRIES = 10;
    static constexpr uint32_t CHECK_INTERVAL_MS = 1000;

    EventQueue &evQ;
    Timer checkTimer{CHECK_INTERVAL_MS};
    uint8_t tries = MAX_TRIES;
};