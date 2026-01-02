
#pragma once

#include "config.h"
#include "EventQueue.h"
#include "LampState.h"

#include <WebSocketsServer.h>

class WebSocketController
{
public:
  WebSocketController(EventQueue &eventQueue, uint16_t port = 81) : evQ(eventQueue), ws(port)
  {
    ws.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
               { this->handleWebSocketEvent(num, type, payload, length); });
  }

  void init() { ws.begin(); }
  void drop() { ws.close(); }

  void tick() { ws.loop(); }

  void update()
  {
    String message;
    message.reserve(140);

    message += F("IP:");
    message += lampState.wifiMode == LampWiFiMode::STA ? (String(lampState.localIPSTA[0]) + "." +
                                                          String(lampState.localIPSTA[1]) + "." +
                                                          String(lampState.localIPSTA[2]) + "." +
                                                          String(lampState.localIPSTA[3]))
                                                       : (String(lampState.localIPAP[0]) + "." +
                                                          String(lampState.localIPAP[1]) + "." +
                                                          String(lampState.localIPAP[2]) + "." +
                                                          String(lampState.localIPAP[3]));
    message += F("|");

    message += F("BRIGHT:");
    message += effSets[lampState.effIndex].brightness;
    message += F("|");

    message += F("SCALE:");
    message += effSets[lampState.effIndex].scale;
    message += F("|");

    message += F("SPEED:");
    message += effSets[lampState.effIndex].speed;
    message += F("|");

    message += F("SAVED_STA:");
    message += lampState.ssidSTA;
    message += F(",");
    message += lampState.passSTA;

    ws.broadcastTXT(message);
  }

private:
  EventQueue &evQ;

  WebSocketsServer ws;

  void handleWebSocketEvent(uint8_t clientId, WStype_t type, uint8_t *payload, size_t length)
  {
    switch (type)
    {
    case WStype_DISCONNECTED:
      onDisconnect(clientId);
      break;

    case WStype_CONNECTED:
    {
      IPAddress ip = ws.remoteIP(clientId);
      onConnect(clientId, ip);
    }
    break;

    case WStype_TEXT:
      onMessage(clientId, payload, length);
      break;

    case WStype_BIN:
      onBinaryMessage(clientId, payload, length);
      break;

    case WStype_PING:
    case WStype_PONG:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
    }
  }
  virtual void onConnect(uint8_t clientId, IPAddress ip)
  {
    update();
  }

  virtual void onDisconnect(uint8_t clientId)
  {
    // queue message
  }

  virtual void onMessage(uint8_t clientId, uint8_t *payload, size_t length)
  {

    String message = String((char *)payload).substring(0, length);

    if (message.startsWith("POWER"))
      evQ.post(Event::ev(EventType::POWER_CHANGE));
    else if (message.startsWith("EFFECT:"))
      evQ.post(Event::evInt16(EventType::EFF_CHANGE, constrain(message.substring(7).toInt(), 0, lampState.effAmount - 1)));
    else if (message.startsWith("BRIGHT:"))
      evQ.post(Event::evInt16(EventType::EFF_SET_BRIGHTNESS, constrain(message.substring(7).toInt(), 1, 255)));
    else if (message.startsWith("SCALE:"))
      evQ.post(Event::evInt16(EventType::EFF_SET_SCALE, constrain(message.substring(6).toInt(), 1, 255)));
    else if (message.startsWith("SPEED:"))
      evQ.post(Event::evInt16(EventType::EFF_SET_SPEED, constrain(message.substring(6).toInt(), 10, 255)));
    else if (message.startsWith("STA:"))
    {
      if (lampState.wifiMode == LampWiFiMode::STA)
        return;

      String payload = message.substring(4);
      int8_t commaIndex = payload.indexOf(',');

      if (commaIndex == -1)
        return;

      payload.substring(0, commaIndex)
          .toCharArray(lampState.ssidSTA, WIFI_SSID_LEN);

      payload.substring(commaIndex + 1)
          .toCharArray(lampState.passSTA, WIFI_PASS_LEN);
      // lampState.ssidSTA.trim();
      // lampState.passSTA.trim();
      lampState.wifiMode = LampWiFiMode::STA;
      evQ.post(Event::ev(EventType::WIFI_UPDATE));
    }
    else if (message.startsWith("AP"))
    {
      if (lampState.wifiMode == LampWiFiMode::AP)
        return;

      lampState.wifiMode = LampWiFiMode::AP;
      evQ.post(Event::ev(EventType::WIFI_UPDATE));
    }
  }

  virtual void onBinaryMessage(uint8_t clientId, uint8_t *payload, size_t length)
  {
    // queue message
  }
};