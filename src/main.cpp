
#include "config.h"

#include "WiFiController.h"
#include "HttpController.h"
#include "WebSocketController.h"
#include "Effect.h"
#include "EffectsController.h"
#include "EventBus.h"
#include "LampCore.h"
#include "Button.h"
#include "ButtonController.h"
#include "LedConfiguration.h"

Effect *effects[EFFECTS_AMOUNT];
EffectsController effectsManager(EFFECTS_AMOUNT, effects);

ButtonController lampBtn(BTN_PIN, false);

CRGB leds[LED_AMOUNT];

WiFiController lampWiFi(
    0, // 0 - STA, 1 - AP
    // ========== STA mode settings ==========
    "keenuka",
    "ZreTHEA44",
    // ========== AP mode settings ==========
    IPAddress(192, 168, 4, 1),
    IPAddress(192, 168, 4, 1),
    IPAddress(255, 255, 255, 0),
    "Lamp",
    "31415926");

HttpController lampHttpServer(80);
WebSocketController lampWebSocket(81);

#if MATRIX_TYPE == 2
DoublePanelSnakeMatrix ledConfig(leds, WIDTH, HEIGHT);
#elif MATRIX_TYPE == 1
SnakeMatrix ledConfig(leds, WIDTH, HEIGHT);
#elif MATRIX_TYPE == 0
RowMatrix ledConfig(leds, WIDTH, HEIGHT);
#endif

LampCore core;

void setup()
{
  effects[0] = new Effect(ledConfig);        // Red
  effects[1] = new RainbowEffect(ledConfig); // Rainbow Vertical

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

  lampWiFi.initWiFi();
  lampHttpServer.initHttpServer();
}

void loop()
{

  effectsManager.tick();

  lampHttpServer.tick();
  yield();
}
