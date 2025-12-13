
#include "config.h"

#include "WiFiController.h"
#include "HttpController.h"
#include "WebSocketController.h"
#include "Effect.h"
#include "EffectsController.h"
#include "EventQueue.h"
#include "LampCore.h"
#include "Button.h"
#include "ButtonController.h"
#include "LedConfiguration.h"
#include "LampState.h"

CRGB leds[LED_AMOUNT];

EventQueue eventQueue;

ButtonController lampBtn(eventQueue, BTN_PIN, false);

WiFiController WiFiCtrl(
    0, // 0 - STA, 1 - AP
    // ========== STA mode settings ==========
    "keenuka",
    "ZreTHEA44",
    // ========== AP mode settings ==========
    IPAddress(192, 168, 4, 1),
    IPAddress(192, 168, 4, 1),
    IPAddress(255, 255, 255, 0),
    "Lamp",
    "31415926",
    eventQueue);

HttpController httpCtrl(HTTP_PORT);
WebSocketController webSocketCtrl(eventQueue, WS_PORT);

#if MATRIX_TYPE == 2
DoublePanelSnakeMatrix ledConfig(leds, WIDTH, HEIGHT);
#elif MATRIX_TYPE == 1
SnakeMatrix ledConfig(leds, WIDTH, HEIGHT);
#elif MATRIX_TYPE == 0
RowMatrix ledConfig(leds, WIDTH, HEIGHT);
#endif

Effect *effects[EFFECTS_AMOUNT];
Effect testDotEff(ledConfig);
RainbowEffect rainbowEff(ledConfig);

EffectsController effectsCtrl(effects);

LampCore core(eventQueue, effectsCtrl, webSocketCtrl);

void setup()
{

  Serial.println();
  Serial.println(F("===== BOOT INFO ====="));
  Serial.println(ESP.getResetInfo());
  Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
  Serial.println(F("====================="));

  effects[0] = &testDotEff; // Red
  effects[1] = &rainbowEff; // Rainbow Vertical

  Serial.begin(115200);
  delay(2000);

  LittleFS.begin();

  Dir dir = LittleFS.openDir("/");
  while (dir.next())
  {
    Serial.printf("FILE: %s (%d bytes)\n",
                  dir.fileName().c_str(),
                  dir.fileSize());
  }

  // LED setup
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COL_ORDER>(leds, LED_AMOUNT);
  FastLED.setBrightness(0);
  FastLED.clear(true);

  // Check led buffers pointers
  Serial.println((uint32_t)leds, HEX);
  Serial.println((uint32_t)ledConfig._leds, HEX);

  WiFiCtrl.init();
  httpCtrl.init();
  webSocketCtrl.init();

  effectsCtrl.init();

  core.init();
}

void loop()
{

  WiFiCtrl.tick();
  yield();

  httpCtrl.tick();
  webSocketCtrl.tick();
  yield();

#if USE_BTN == 1
  lampBtn.tick();
#endif

  effectsCtrl.tick();

  for (uint8_t i = 0; i < CORE_MESSAGES_PER_TICK; i++)
  {
    core.tick();
  }
}
