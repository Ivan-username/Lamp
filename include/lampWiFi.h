#pragma once

#include "config.h"
#include "iconViewer.h"

class LampWiFi
{
public:
    LampWiFi(uint8_t wifiMode,
             String STAssid,
             String STApassword,
             IPAddress local_ip,
             IPAddress gateway,
             IPAddress subnet,
             String APssid = "LampAP",
             String APpassword = "31415926")
        : _wifiMode(wifiMode),
          _STAssid(STAssid),
          _STApassword(STApassword),
          _local_ip(local_ip),
          _gateway(gateway),
          _subnet(subnet),
          _APssid(APssid),
          _APpassword(APpassword)
    {
    }

    void initWiFi()
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
    }

    void setSTAMode()
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(_STAssid, _STApassword);
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

    uint8_t _wifiMode;
    String _STAssid;
    String _STApassword;
    IPAddress _local_ip;
    IPAddress _gateway;
    IPAddress _subnet;
    String _APssid;
    String _APpassword;
};