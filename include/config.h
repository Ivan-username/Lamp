#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

// WiFi configuration
const char *ssid = "keenuka";
const char *password = "ZreTHEA44";

// Server configuration
#define SERVER_PORT 80

// Effect configuration
#define MODE_AMOUNT 3
uint8_t currentModeID = 0;

// LED matrix configuration
#define WIDTH 16
#define HEIGHT 16
#define LED_NUM (WIDTH * HEIGHT)
#define SEGMENTS 1
#define MATRIX_TYPE 0      // 0-змейка 1-рядками
#define CONNECTION_ANGLE 1 // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 0  // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз

boolean effectSlowStart = true;

struct
{
    byte brightness = 100;
    byte speed = 30;
    byte scale = 40;
} modes[MODE_AMOUNT];

// Global variables
CRGB leds[LED_NUM];
ESP8266WebServer server(SERVER_PORT);