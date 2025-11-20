#pragma once

#include "config.h"
#include "lampWiFi.h"
#include "htmls.h"

void handlerRoot()
{
    server.send(200, "text/html", getHTML());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
        Serial.println(" Disconnected!");
        break;
    }
    case WStype_CONNECTED:
    {
        webSocket.sendTXT(num, isOn ? "state:on" : "state:off");
        webSocket.sendTXT(num, ("brightness:" + String(modes[currentModeID].brightness)).c_str());
        webSocket.sendTXT(num, ("scale:" + String(modes[currentModeID].scale)).c_str());
        webSocket.sendTXT(num, ("speed:" + String(modes[currentModeID].speed)).c_str());
    }
    break;
    case WStype_TEXT:
    {
        // Обработка команд
        if (String((char *)payload).startsWith("state"))
        {
            isOn = !isOn;
            effectSlowStart = true;
            Serial.println("State: " + isOn ? "On" : "Off");
        }
        else if (String((char *)payload).startsWith("mode:"))
        {
            String val = String((char *)payload).substring(5);
            currentModeID = val.toInt();
            effectSlowStart = true;
            webSocket.sendTXT(num, ("brightness:" + String(modes[currentModeID].brightness)).c_str());
            webSocket.sendTXT(num, ("scale:" + String(modes[currentModeID].scale)).c_str());
            webSocket.sendTXT(num, ("speed:" + String(modes[currentModeID].speed)).c_str());
            Serial.println("Mode: " + val);
        }
        else if (String((char *)payload).startsWith("brightness:"))
        {
            String val = String((char *)payload).substring(11);
            modes[currentModeID].brightness = val.toInt();
            Serial.println("Brightness: " + val);
        }
        else if (String((char *)payload).startsWith("scale:"))
        {
            String val = String((char *)payload).substring(6);
            modes[currentModeID].scale = val.toInt();
            Serial.println("Scale: " + val);
        }
        else if (String((char *)payload).startsWith("speed:"))
        {
            String val = String((char *)payload).substring(6);
            modes[currentModeID].speed = val.toInt();
            Serial.println("Speed: " + val);
        }
        else if (String((char *)payload).startsWith("wifi:"))
        {
            String val = String((char *)payload).substring(5);
            int separatorIndex = val.indexOf(",");
            String ssid = val.substring(0, separatorIndex);
            String password = val.substring(separatorIndex + 1);

            config.wifiMode = 0;
            config.STAssid = ssid;
            config.STApassword = password;
            data.updateNow();
            delay(1000);
            ESP.restart();
            Serial.println("Recived new wifi config:" + ssid + ":" + password);
        }
        break;
    }
    default:
    {
        Serial.println("WebSocket type received: " + String(type));
        break;
    }
    }
}

void setupHttpServer()
{

    server.on("/", handlerRoot);
    server.begin();
    delay(1000);
    Serial.println("HTTP server started.");
}

void setupWebsocketServer()
{

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    delay(1000);
    Serial.println("WebSocket server started.");
}
