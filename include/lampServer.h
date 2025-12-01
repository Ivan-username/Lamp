#pragma once

#include "config.h"
#include "lampWiFi.h"
#include "lampLedUtils.h"

void handleIndex()
{
    File f = LittleFS.open("/index.html", "r");
    if (!f)
    {
        server.send(404, "text/plain", "index.html not found");
        return;
    }
    server.streamFile(f, "text/html");
    f.close();
}

void handleCSS()
{
    File f = LittleFS.open("/style.css", "r");
    if (!f)
    {
        server.send(404, "text/plain", "style.css not found");
        return;
    }
    server.streamFile(f, "text/css");
    f.close();
}

void handleJS()
{
    File f = LittleFS.open("/script.js", "r");
    if (!f)
    {
        server.send(404, "text/plain", "script.js not found");
        return;
    }
    server.streamFile(f, "application/javascript");
    f.close();
}

void handleSvg()
{
    String path = server.uri(); // /icons/ip.svg

    File f = LittleFS.open(path, "r");
    if (!f)
    {
        server.send(404, "text/plain", "SVG not found");
        return;
    }
    server.streamFile(f, "image/svg+xml");
    f.close();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
        DEBUGLN(" Disconnected!");
        break;
    }
    case WStype_CONNECTED:
    {
        webSocket.sendTXT(num, ("ip:" + (config.wifiMode ? local_ip.toString() : WiFi.localIP().toString())).c_str());
        webSocket.sendTXT(num, ("brightness:" + String(modes[currentModeID].brightness)).c_str());
        webSocket.sendTXT(num, ("scale:" + String(modes[currentModeID].scale)).c_str());
        webSocket.sendTXT(num, ("speed:" + String(modes[currentModeID].speed)).c_str());
        webSocket.sendTXT(num, (String("savedSTA:") + config.STAssid + "," + config.STApassword).c_str());
    }
    break;
    case WStype_TEXT:
    {
        // Обработка команд
        if (String((char *)payload).startsWith("switch"))
        {
            isOn = !isOn;
            effectSlowStart = true;
            DEBUGLN("switch: " + isOn ? "On" : "Off");
        }
        else if (String((char *)payload).startsWith("effect:"))
        {
            String val = String((char *)payload).substring(7);
            currentModeID = val.toInt();
            effectSlowStart = true;
            webSocket.sendTXT(num, ("brightness:" + String(modes[currentModeID].brightness)).c_str());
            webSocket.sendTXT(num, ("scale:" + String(modes[currentModeID].scale)).c_str());
            webSocket.sendTXT(num, ("speed:" + String(modes[currentModeID].speed)).c_str());
            DEBUGLN("Mode: " + val);
        }
        else if (String((char *)payload).startsWith("brightness:"))
        {
            String val = String((char *)payload).substring(11);
            modes[currentModeID].brightness = val.toInt();
            DEBUGLN("Brightness: " + val);
        }
        else if (String((char *)payload).startsWith("scale:"))
        {
            String val = String((char *)payload).substring(6);
            modes[currentModeID].scale = val.toInt();
            DEBUGLN("Scale: " + val);
        }
        else if (String((char *)payload).startsWith("speed:"))
        {
            String val = String((char *)payload).substring(6);
            modes[currentModeID].speed = val.toInt();
            DEBUGLN("Speed: " + val);
        }
        else if (String((char *)payload).startsWith("sta:"))
        {
            String val = String((char *)payload).substring(4);
            int separatorIndex = val.indexOf(",");
            String ssid = val.substring(0, separatorIndex);
            String password = val.substring(separatorIndex + 1);

            config.wifiMode = 0;
            config.STAssid = ssid;
            config.STApassword = password;
            data.updateNow();
            iconAnimation(&rebootIcon[0][0], CRGB::Red);
            ESP.restart();
            DEBUGLN("Recived new wifi config:" + ssid + ":" + password);
        }
        else if (String((char *)payload).startsWith("ap"))
        {
            config.wifiMode = 1;
            data.updateNow();
            iconAnimation(&rebootIcon[0][0], CRGB::Red);
            ESP.restart();
            DEBUGLN("Switched to ap mode");
        }
        else if (String((char *)payload).startsWith("reboot"))
        {
            iconAnimation(&rebootIcon[0][0], CRGB::Red);
            ESP.restart();
        }
        break;
    }
    case WStype_PING:
    {
        DEBUGLN("WebSocket ping");
    }
    default:
    {
        DEBUGLN("WebSocket type received: " + String(type));
        break;
    }
    }
}

void setupHttpServer()
{

    server.on("/", handleIndex);
    server.on("/index.html", handleIndex);

    server.on("/style.css", handleCSS);
    server.on("/script.js", handleJS);

    server.on("/icons/ip.svg", handleSvg);
    server.on("/icons/reload.svg", handleSvg);
    server.on("/icons/brightness.svg", handleSvg);
    server.on("/icons/scale.svg", handleSvg);
    server.on("/icons/speed.svg", handleSvg);
    server.on("/icons/onoff.svg", handleSvg);
    server.on("/icons/options.svg", handleSvg);
    server.on("/icons/sta.svg", handleSvg);
    server.on("/icons/ap.svg", handleSvg);
    server.on("/icons/reboot.svg", handleSvg);

    server.begin();
    iconAnimation(&httpIcon[0][0], CRGB::White);
    DEBUGLN("HTTP server started.");
}

void setupWebsocketServer()
{

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    iconAnimation(&webSocketIcon[0][0], CRGB::White);
    DEBUGLN("WebSocket server started.");
}
