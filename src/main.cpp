
#include "config.h"

#include <FileData.h>

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
#include "Animation.h"

CRGB leds[LED_AMOUNT];

EventQueue eventQueue;

ButtonController lampBtn(eventQueue, BTN_PIN, false);

WiFiController wifiCtrl(eventQueue);

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
Effect testDot(ledConfig);
JustLampEffect justLampEff(ledConfig);
// FireNoiseEffect fireNoiseEffect(ledConfig);
GyverFireEffect gyverFireEff(ledConfig);
RainbowHorizontalEffect rainbowHorzEff(ledConfig);
RainbowVerticalEffect rainbowVertEff(ledConfig);
SnowfallEffect snowfallEffect(ledConfig);

EffectsController effectsCtrl(effects);

Animation *animations[1];
Animation testAnim(ledConfig);

LampCore core(eventQueue, effectsCtrl, webSocketCtrl, httpCtrl, wifiCtrl);

void setup()
{

  effects[0] = &justLampEff;
  effects[1] = &gyverFireEff;
  effects[2] = &rainbowHorzEff;
  effects[3] = &rainbowVertEff;
  effects[4] = &snowfallEffect;

  animations[0] = &testAnim;

  Serial.begin(115200);
  delay(2000);

  LittleFS.begin();

#ifdef USE_EEPROM
  lampStateFD.read();
  effSetsFD.read();
#endif

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

  wifiCtrl.init();
  httpCtrl.init();
  webSocketCtrl.init();

  effectsCtrl.init();

  core.init();
}

void loop()
{

  wifiCtrl.tick();
  yield();

  httpCtrl.tick();
  webSocketCtrl.tick();
  yield();

#ifdef USE_BTN
  lampBtn.tick();
#endif

  effectsCtrl.tick();

  for (uint8_t i = 0; i < CORE_MESSAGES_PER_TICK; i++)
  {
    core.tick();
  }
#ifdef USE_EEPROM
  if (lampStateFD.tick() == FD_WRITE || effSetsFD.tick() == FD_WRITE)
    DEBUGLN("EEPROM UPDATED");
#endif
}
