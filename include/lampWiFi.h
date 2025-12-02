#pragma once

#include "config.h"
#include "iconViewer.h"

static byte reconnectionTries = 10;

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void setupAPMode()
{

    WiFi.mode(WIFI_AP);
    WiFi.softAP(APssid, APpassword);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    DEBUG("Access Point IP address: ");
    DEBUGLN(WiFi.softAPIP());
    iconAnimation(&apIcon[0][0], CRGB::Green, INFO_SERV_ANIMATION);
}

void setupSTAMode()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.STAssid, config.STApassword);

    while (--reconnectionTries && WiFi.status() != WL_CONNECTED)
    {
        iconAnimation(&staIcon[0][0], CRGB::White, LOAD_SERV_ANIMATION);
        DEBUGLN(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        iconAnimation(&staIcon[0][0], CRGB::Green, INFO_SERV_ANIMATION);
        DEBUG("IP address: ");
        DEBUGLN(WiFi.localIP());
    }
    else
    {
        iconAnimation(&staIcon[0][0], CRGB::Red, INFO_SERV_ANIMATION);
        WiFi.disconnect();
        setupAPMode();
    }
}

void setupWiFi()
{
    if (config.wifiMode == 0)
        setupSTAMode();
    else
        setupAPMode();
}