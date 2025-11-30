#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <FileData.h>
#include <LittleFS.h>

const char *APssid = "Lamp";
const char *APpassword = "12345678";

// Server configuration
#define SERVER_PORT 80

// Effect configuration
#define MODE_AMOUNT 2
uint8_t currentModeID = 0;

// LED matrix configuration
#define WIDTH 16
#define HEIGHT 8
#define LED_AMOUNT (WIDTH * HEIGHT)
#define MATRIX_TYPE 2 // 0-рядками 1-змейка 2-(8х8)х2

// Configuration structure
struct Config
{
    uint8_t wifiMode = 0; // 0 - STA, 1 - AP
    String STAssid = "keenuka";
    String STApassword = "ZreTHEA43";
};
Config config;

FileData data(&LittleFS, "/data.dat", 'A', &config, sizeof(config));

// Global variables
struct
{
    byte brightness = 50;
    byte speed = 30;
    byte scale = 40;
} modes[MODE_AMOUNT];

boolean effectSlowStart = true;
boolean isOn = false;

// Objects
CRGB leds[LED_AMOUNT];
WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket сервер на порту 80
ESP8266WebServer server(80);                       // HTTP сервер на порту 80

#define FOR_U8_I(x, y) for (uint8_t i = (x); i < (y); i++)
#define FOR_U8_J(x, y) for (uint8_t j = (x); j < (y); j++)
#define FOR_U8_K(x, y) for (uint8_t k = (x); k < (y); k++)
#define FOR_U16_I(x, y) for (uint16_t i = (x); i < (y); i++)
#define FOR_U16_J(x, y) for (uint16_t j = (x); j < (y); j++)
#define FOR_U16_K(x, y) for (uint16_t k = (x); k < (y); k++)

#define DEBUG_SERIAL_LAMP 1
#ifdef DEBUG_SERIAL_LAMP
#define DEBUGLN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUGLN(x)
#define DEBUG(x)
#endif