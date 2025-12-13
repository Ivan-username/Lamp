#pragma once

#include "config.h"
#include "EventQueue.h"
#include "StepTimer.h"

#include <ESP8266WiFi.h>

class WiFiController
{
public:
    WiFiController(uint8_t wifiMode,
                   String STAssid,
                   String STApassword,
                   IPAddress local_ip,
                   IPAddress gateway,
                   IPAddress subnet,
                   String APssid,
                   String APpassword,
                   EventQueue &eventQueue)
        : _wifiMode(wifiMode),
          _STAssid(STAssid),
          _STApassword(STApassword),
          _local_ip(local_ip),
          _gateway(gateway),
          _subnet(subnet),
          _APssid(APssid),
          _APpassword(APpassword),
          evQ(eventQueue)
    {
    }

    void tick()
    {
        if (_wifiMode == 0)
        {
            if (getSTAstatus() == WL_CONNECTED && !_STAStatus)
                evQ.post(Event::ev(EventType::WIFI_CONNECTED));
            else
            {
                evQ.post(Event::ev(EventType::WIFI_DISCONNECTED));
                _STAStatus = false;
                _STAReconnectTimer.restart();
            }

            if (!_STAStatus && _STAReconnectTimer.isReady())
            {
                if (_STAreconnectTries > 0)
                {
                    _STAreconnectTries--;
                    _STAReconnectTimer.reset();
                }
                else
                {
                    evQ.post(Event::ev(EventType::WIFI_SWITCH_TO_AP));
                    _STAStatus = true; // to prevent multiple events
                    _STAreconnectTries = 10;
                    _wifiMode = 1;
                    _STAReconnectTimer.stop();
                }
            }
        }
    }

    void init()
    {
        if (_wifiMode)
            setAPMode();
        else
            setSTAMode();
    }

    void setAPMode()
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(_APssid, _APpassword);
        WiFi.softAPConfig(_local_ip, _gateway, _subnet);
        // queue message
    }

    void setSTAMode()
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(_STAssid, _STApassword);
        // queue message
    }

    wl_status_t getSTAstatus()
    {
        return WiFi.status();
    }

    IPAddress getSTALocalIP()
    {
        return WiFi.localIP();
    }

    IPAddress getAPLocalIP()
    {
        return WiFi.softAPIP();
    }

private:
    uint8_t _wifiMode;
    String _STAssid;
    String _STApassword;
    IPAddress _local_ip;
    IPAddress _gateway;
    IPAddress _subnet;
    String _APssid = "Lamp";
    String _APpassword = "31415926";
    EventQueue &evQ;

    StepTimer _STAReconnectTimer = StepTimer(1000, true);
    bool _STAStatus = false;
    uint8_t _STAreconnectTries = 10;
};