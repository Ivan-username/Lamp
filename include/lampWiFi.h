#pragma once

#include "config.h"

static byte reconnectionTries = 10;

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void setupAPMode()
{

    WiFi.mode(WIFI_AP);
    WiFi.softAP(APssid, APpassword);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP());
    delay(1000);
}

void setupSTAMode()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.STAssid, config.STApassword);

    while (--reconnectionTries && WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        WiFi.disconnect();
        setupAPMode();
    }
}

void setupWiFi()
{
    if (config.wifiMode == 0)
    {
        setupSTAMode();
    }
    else
    {
        setupAPMode();
    }
}