#pragma once

#include "config.h"
#include "EventQueue.h"
#include "Timer.h"
#include "LampState.h"

#include <ESP8266WiFi.h>

class WiFiController
{
public:
    WiFiController(EventQueue &eventQueue) : evQ(eventQueue)
    {
    }

    void tick()
    {
        if (!checkTimer.isReady())
            return;

        if (lampState.wifiMode != LampWiFiMode::STA)
            return;

        wl_status_t status = WiFi.status();

        if (status == WL_CONNECTED)
        {
            if (!connected)
            {
                connected = true;
                evQ.post(Event::ev(EventType::WIFI_CONNECTED));
            }
            return;
        }

        // ---- NOT CONNECTED ----

        if (connected)
        {
            connected = false;
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
        setWiFiMode(lampState.wifiMode);
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
    bool connected = false;
    uint8_t tries = MAX_TRIES;
};