#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <FileData.h>
#include <LittleFS.h>
#include <Button.h>

// Service icons duaration (ms) configuration
#define LOAD_SERV_ANIMATION 1000
#define INFO_SERV_ANIMATION 300

// Server configuration
#define SERVER_PORT 80

// Effect configuration
#define EFFECTS_AMOUNT 2

// LED configuration
#define LED_TYPE WS2812
#define LED_PIN D4
#define LED_COL_ORDER GRB

#define WIDTH 16
#define HEIGHT 8
#define LED_AMOUNT (WIDTH * HEIGHT)
#define MATRIX_TYPE 2 // 0 - [rows], 1 - [snake], 2 - [(nхn)х2 snake]

// Button configuration
#define USE_BTN 1
#define BTN_PIN D2

// Debugging
#define DEBUG_SERIAL_LAMP
#ifdef DEBUG_SERIAL_LAMP
#define DEBUGLN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUGLN(x)
#define DEBUG(x)
#endif