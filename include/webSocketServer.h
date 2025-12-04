#pragma once

#include "config.h"
#include "lampWiFi.h"
#include "effectsTicker.h"
#include "iconViewer.h"

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
    webSocket.sendTXT(num, ("brightness:" + String(effects[currentEffectID]->_brightness)).c_str());
    webSocket.sendTXT(num, ("scale:" + String(effects[currentEffectID]->_scale)).c_str());
    webSocket.sendTXT(num, ("speed:" + String(effects[currentEffectID]->_speed)).c_str());
    webSocket.sendTXT(num, (String("savedSTA:") + config.STAssid + "," + config.STApassword).c_str());
  }
  break;
  case WStype_TEXT:
  {
    // Commands checking
    if (String((char *)payload).startsWith("switch"))
    {
      isOn = !isOn;
      changeEffect(currentEffectID);
      DEBUGLN("switch: " + isOn ? "On" : "Off");
    }
    else if (String((char *)payload).startsWith("effect:"))
    {
      String val = String((char *)payload).substring(7);
      changeEffect(val.toInt());
      webSocket.sendTXT(num, ("brightness:" + String(effects[currentEffectID]->_brightness)).c_str());
      webSocket.sendTXT(num, ("scale:" + String(effects[currentEffectID]->_scale)).c_str());
      webSocket.sendTXT(num, ("speed:" + String(effects[currentEffectID]->_speed)).c_str());
    }
    else if (String((char *)payload).startsWith("brightness:"))
    {
      String val = String((char *)payload).substring(11);
      effects[currentEffectID]->_brightness = val.toInt();
      DEBUGLN("Brightness: " + val);
    }
    else if (String((char *)payload).startsWith("scale:"))
    {
      String val = String((char *)payload).substring(6);
      effects[currentEffectID]->_scale = val.toInt();
      DEBUGLN("Scale: " + val);
    }
    else if (String((char *)payload).startsWith("speed:"))
    {
      String val = String((char *)payload).substring(6);
      effects[currentEffectID]->_speed = val.toInt();
      DEBUGLN("Speed: " + val);
    }
    else if (String((char *)payload).startsWith("sta:"))
    {
      String val = String((char *)payload).substring(4);
      uint8_t separatorIndex = val.indexOf(",");
      String ssid = val.substring(0, separatorIndex);
      String password = val.substring(separatorIndex + 1);
      ssid.trim();
      password.trim();
      updateSTA(ssid, password);
      break;
    }
    else if (String((char *)payload).startsWith("ap"))
    {
      updateAP();
      break;
    }
    else if (String((char *)payload).startsWith("reboot"))
    {
      iconAnimation(rebootIcon, CRGB::Red, LOAD_SERV_ANIMATION);
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
  webSocket.sendTXT(num, "ok"); // for connection status bar
}

void setupWebSocketServer()
{
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  iconAnimation(webSocketIcon, CRGB::White, 500);
  DEBUGLN("WebSocket server started.");
}
