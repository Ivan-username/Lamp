#pragma once

#include "config.h"

static byte reconnectionTries = 10;

void setupWiFi()
{
    WiFi.begin(ssid, password);

    while (--reconnectionTries && WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        // Иначе удалось подключиться отправляем сообщение
        Serial.println("IP address: ");
        Serial.print(WiFi.localIP());
        digitalWrite(LED_BUILTIN, 0);
    }
    else
    {
        Serial.println("Non Connecting to WiFi..");
    }
}