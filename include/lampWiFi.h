#pragma once

#include "config.h"
#include "iconViewer.h"

static byte reconnectionTries = 10;

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void updateAP()
{
    if (config.wifiMode == 1)
    {
        iconAnimation(apIcon, CRGB::Green, INFO_SERV_ANIMATION);
        return;
    }

    DEBUGLN("Switched to ap mode");
    config.wifiMode = 1;
    data.updateNow();
    iconAnimation(rebootIcon, CRGB::Red, LOAD_SERV_ANIMATION);
    ESP.restart();
}

void updateSTA(String ssid, String password)
{
    DEBUGLN("Recived wifi config:" + ssid + ":" + password);

    if (config.wifiMode == 0 && (ssid.isEmpty() || password.isEmpty()))
    {
        iconAnimation(staIcon, CRGB::Green, INFO_SERV_ANIMATION);
        return;
    }
    else if (config.wifiMode == 1 && (ssid.isEmpty() || password.isEmpty()))
    {
        config.wifiMode = 0;
        data.updateNow();
        DEBUGLN("STA mode with earlier saved config");
        iconAnimation(rebootIcon, CRGB::Red, LOAD_SERV_ANIMATION);
        ESP.restart();
    }

    config.wifiMode = 0;
    config.STAssid = ssid;
    config.STApassword = password;
    data.updateNow();
    iconAnimation(rebootIcon, CRGB::Red, LOAD_SERV_ANIMATION);
    ESP.restart();
}

void setupAPMode()
{

    WiFi.mode(WIFI_AP);
    WiFi.softAP(APssid, APpassword);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    DEBUG("Access Point IP address: ");
    DEBUGLN(WiFi.softAPIP());
    iconAnimation(apIcon, CRGB::Green, INFO_SERV_ANIMATION);
}

void setupSTAMode()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.STAssid, config.STApassword);

    while (--reconnectionTries && WiFi.status() != WL_CONNECTED)
    {
        iconAnimation(staIcon, CRGB::White, LOAD_SERV_ANIMATION);
        DEBUGLN(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        iconAnimation(staIcon, CRGB::Green, INFO_SERV_ANIMATION);
        DEBUG("IP address: ");
        DEBUGLN(WiFi.localIP());
    }
    else
    {
        iconAnimation(staIcon, CRGB::Red, INFO_SERV_ANIMATION);
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