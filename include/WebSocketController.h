
// {
//   switch (type)
//   {
//   case WStype_DISCONNECTED:
//   {
//     DEBUGLN(" Disconnected!");
//     break;
//   }
//   case WStype_CONNECTED:
//   {
//     // webSocket.sendTXT(num, ("ip:" + (config.wifiMode ? local_ip.toString() : WiFi.localIP().toString())).c_str());
//     webSocket.sendTXT(num, ("brightness:" + String(effects[currentEffectID]->_brightness)).c_str());
//     webSocket.sendTXT(num, ("scale:" + String(effects[currentEffectID]->_scale)).c_str());
//     webSocket.sendTXT(num, ("speed:" + String(effects[currentEffectID]->_speed)).c_str());
//     // webSocket.sendTXT(num, (String("savedSTA:") + config.STAssid + "," + config.STApassword).c_str());
//   }
//   break;
//   case WStype_TEXT:
//   {
//     // Commands checking
//     if (String((char *)payload).startsWith("switch"))
//     {
//       isOn = !isOn;
//       effectsManager->setEffect(currentEffectID);
//       DEBUGLN("switch: " + isOn ? "On" : "Off");
//     }
//     else if (String((char *)payload).startsWith("effect:"))
//     {
//       String val = String((char *)payload).substring(7);
//       effectsManager->setEffect(val.toInt());
//       webSocket.sendTXT(num, ("brightness:" + String(effects[currentEffectID]->_brightness)).c_str());
//       webSocket.sendTXT(num, ("scale:" + String(effects[currentEffectID]->_scale)).c_str());
//       webSocket.sendTXT(num, ("speed:" + String(effects[currentEffectID]->_speed)).c_str());
//     }
//     else if (String((char *)payload).startsWith("brightness:"))
//     {
//       String val = String((char *)payload).substring(11);
//     }
//     else if (String((char *)payload).startsWith("scale:"))
//     {
//       String val = String((char *)payload).substring(6);
//     }
//     else if (String((char *)payload).startsWith("speed:"))
//     {
//       String val = String((char *)payload).substring(6);
//     }
//     else if (String((char *)payload).startsWith("sta:"))
//     {
//       String val = String((char *)payload).substring(4);
//       uint8_t separatorIndex = val.indexOf(",");
//       String ssid = val.substring(0, separatorIndex);
//       String password = val.substring(separatorIndex + 1);
//       ssid.trim();
//       password.trim();
//       break;
//     }
//     else if (String((char *)payload).startsWith("ap"))
//     {
//       // updateAP();
//       break;
//     }
//     else if (String((char *)payload).startsWith("reboot"))
//     {
//     }
//     break;
//   }
//   case WStype_PING:
//   {
//     DEBUGLN("WebSocket ping");
//   }
//   default:
//   {
//     DEBUGLN("WebSocket type received: " + String(type));
//     break;
//   }
//   }
//   webSocket.sendTXT(num, "ok"); // for connection status bar
// }

#pragma once

#include "config.h"
#include <WebSocketsServer.h>
#include "EventBus.h"
#include "LampState.h"

class WebSocketController
{
public:
  WebSocketController(EventBus &bus, uint16_t port = 81)
      : bus(bus), ws(port) {}

  void init()
  {
    ws.begin();
    ws.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t len)
               { handle(num, type, payload, len); });

    // подписка на изменения состояния
    bus.subscribe([this](const Event &ev)
                  {
            if (ev.type == Event::StateChanged)
                broadcastState(); });
  }

  void tick() { ws.loop(); }

private:
  EventBus &bus;
  WebSocketsServer ws;

  void handle(uint8_t num, WStype_t type, uint8_t *payload, size_t len)
  {

    // if (type == WStype_DISCONNECTED)
    // {
    //   DEBUGLN("WebSocket Disconnected!");
    //   return;
    // }
    // if (type == WStype_CONNECTED)
    // {
    //   DEBUGLN("WebSocket Connected!");
    //   broadcastState();
    //   return;
    // }

    if (type != WStype_TEXT)
      return;

    String s = String((char *)payload);

    if (s == "switch")
      bus.publish({Event::TogglePower});
    else if (s.startsWith("effect:"))
      bus.publish({Event::SetEffect, s.substring(7).toInt()});
    else if (s.startsWith("brightness:"))
      bus.publish({Event::SetBrightness, s.substring(11).toInt()});
    else if (s.startsWith("speed:"))
      bus.publish({Event::SetSpeed, s.substring(6).toInt()});
    else if (s.startsWith("scale:"))
      bus.publish({Event::SetScale, s.substring(6).toInt()});
    else if (s.startsWith("sta:"))
      bus.publish({Event::WiFiSTAUpdated, 0, s.substring(4)});
    else if (s.startsWith("ap:"))
      bus.publish({Event::WiFiAPUpdated, 0, s.substring(3)});
    else if (s == "reboot")
      bus.publish({Event::RebootRequested});
  }

  void broadcastState()
  {

    // ws.sendTXT(num, ("brightness:" + String(effects[currentEffectID]->_brightness)).c_str());
    // ws.sendTXT(num, ("scale:" + String(effects[currentEffectID]->_scale)).c_str());
    // ws.sendTXT(num, ("speed:" + String(effects[currentEffectID]->_speed)).c_str());
    // Здесь LampCore отправляет состояние через EventBus в JSON
    // но это будет реализовано ниже через обратный вызов setStateSender()
  }
};