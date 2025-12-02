#pragma once

#include "config.h"
#include "lampWiFi.h"
#include "lampLedUtils.h"

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
    // Commands checking
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
      ssid.trim();
      password.trim();
      DEBUGLN("Recived wifi config:" + ssid + ":" + password);

      if (config.wifiMode == 0 && (ssid.isEmpty() || password.isEmpty()))
      {
        webSocket.sendTXT(num, "ok"); // for connection status bar
        iconAnimation(&staIcon[0][0], CRGB::Green, INFO_SERV_ANIMATION);
        break;
      }
      else if (config.wifiMode == 1 && (ssid.isEmpty() || password.isEmpty()))
      {
        config.wifiMode = 0;
        data.updateNow();
        DEBUGLN("STA mode with earlier saved config");
        iconAnimation(&rebootIcon[0][0], CRGB::Red, LOAD_SERV_ANIMATION);
        ESP.restart();
      }

      config.wifiMode = 0;
      config.STAssid = ssid;
      config.STApassword = password;
      data.updateNow();
      iconAnimation(&rebootIcon[0][0], CRGB::Red, LOAD_SERV_ANIMATION);
      ESP.restart();
    }
    else if (String((char *)payload).startsWith("ap"))
    {
      if (config.wifiMode == 1)
      {
        webSocket.sendTXT(num, "ok"); // for connection status bar
        iconAnimation(&apIcon[0][0], CRGB::Green, INFO_SERV_ANIMATION);
        break;
      }

      config.wifiMode = 1;
      data.updateNow();
      iconAnimation(&rebootIcon[0][0], CRGB::Red, LOAD_SERV_ANIMATION);
      ESP.restart();

      DEBUGLN("Switched to ap mode");
    }
    else if (String((char *)payload).startsWith("reboot"))
    {
      iconAnimation(&rebootIcon[0][0], CRGB::Red, LOAD_SERV_ANIMATION);
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

void setupWebSocketServer()
{
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  iconAnimation(&webSocketIcon[0][0], CRGB::White, 500);
  DEBUGLN("WebSocket server started.");
}
