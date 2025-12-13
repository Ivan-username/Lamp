
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

  void tick() { ws.loop(); }

  void update()
  {
    String message;
    message.reserve(140);

    message += F("IP:");
    message += F("1.1.1.1");
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
      DEBUGLN(type);
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
      Serial.println("WS TEXT");
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
    // queue message
  }

  virtual void onDisconnect(uint8_t clientId)
  {
    // queue message
  }

  virtual void onMessage(uint8_t clientId, uint8_t *payload, size_t length)
  {
    evQ.post(Event::evStr(EventType::WS_MESSAGE,
                          String((char *)payload).substring(0, length)));
    Serial.println(String((char *)payload).substring(0, length));
  }

  virtual void onBinaryMessage(uint8_t clientId, uint8_t *payload, size_t length)
  {
    // queue message
  }
};