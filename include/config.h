#pragma once

// Servers configuration
#define HTTP_PORT 80
#define WS_PORT 81

// Core configuration
#define CORE_MESSAGES_PER_TICK 8

// LED configuration
#define LED_TYPE WS2812
#define LED_PIN D4
#define LED_COL_ORDER GRB

#define WIDTH 16
#define HEIGHT 16
#define LED_AMOUNT (WIDTH * HEIGHT)
#define MATRIX_TYPE 1 // 0 - [rows], 1 - [snake], 2 - [(nхn)х2 snake]

// Button configuration
// #define USE_BTN
#define BTN_PIN D2

// EEPROM
// #define USE_EEPROM
