
#pragma once

#include "config.h"
#include <WebSocketsServer.h>

class WebSocketController
{
public:
  WebSocketController(uint16_t port = 81) : ws(port)
  {
    ws.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
               { this->handleWebSocketEvent(num, type, payload, length); });
  }

  void init() { ws.begin(); }

  void tick() { ws.loop(); }

  void broadcast(const char *message) { ws.broadcastTXT(message); }

private:
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
  }

  virtual void onDisconnect(uint8_t clientId)
  {
  }

  virtual void onMessage(uint8_t clientId, uint8_t *payload, size_t length)
  {
  }

  virtual void onBinaryMessage(uint8_t clientId, uint8_t *payload, size_t length)
  {
  }
};