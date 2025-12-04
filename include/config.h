#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <FileData.h>
#include <LittleFS.h>
#include <Button.h>
#include "Matrix.h"
#include "Effect.h"
#include "Renderer.h"

const char *APssid = "Lamp";
const char *APpassword = "12345678";

// Service icons duaration (ms) configuration
#define LOAD_SERV_ANIMATION 1000
#define INFO_SERV_ANIMATION 300

// Server configuration
#define SERVER_PORT 80

// Effect configuration
#define EFFECTS_AMOUNT 2
uint8_t currentEffectID = 0;
boolean effectSlowStart = true;

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

// Configuration structure
struct Config
{
    uint8_t wifiMode = 0; // 0 - STA, 1 - AP
    String STAssid = "keenuka";
    String STApassword = "ZreTHEA44";
};
Config config;

FileData data(&LittleFS, "/data.dat", 'A', &config, sizeof(config));

// Global variables

boolean isOn = false;

// Objects
CRGB leds[LED_AMOUNT];
WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket сервер на порту 80
ESP8266WebServer server(80);                       // HTTP сервер на порту 80
Button btn(BTN_PIN, false);

#if MATRIX_TYPE == 2
Matrix *matrix = new DoublePanelSnakeMatrix(WIDTH, HEIGHT, leds);
#elif MATRIX_TYPE == 1
Matrix *matrix = new SnakeMatrix(WIDTH, HEIGHT, leds);
#elif MATRIX_TYPE == 0
Matrix *matrix = new RowMatrix(WIDTH, HEIGHT, leds);
#endif

IRenderer *renderer = new MatrixRenderer(matrix);

Effect *effects[EFFECTS_AMOUNT];

#define FOR_U8_I(x, y) for (uint8_t i = (x); i < (y); i++)
#define FOR_U8_J(x, y) for (uint8_t j = (x); j < (y); j++)
#define FOR_U8_K(x, y) for (uint8_t k = (x); k < (y); k++)
#define FOR_U16_I(x, y) for (uint16_t i = (x); i < (y); i++)
#define FOR_U16_J(x, y) for (uint16_t j = (x); j < (y); j++)
#define FOR_U16_K(x, y) for (uint16_t k = (x); k < (y); k++)

// Debugging
#define DEBUG_SERIAL_LAMP
#ifdef DEBUG_SERIAL_LAMP
#define DEBUGLN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUGLN(x)
#define DEBUG(x)
#endif