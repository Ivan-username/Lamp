
#include "config.h"

#include "WiFiController.h"
#include "HttpController.h"
#include "WebSocketController.h"
#include "Effect.h"
#include "EffectData.h"
#include "EffectManager.h"
#include "EventQueue.h"
#include "LampCore.h"
#include "Button.h"
#include "ButtonController.h"
#include "LedConfiguration.h"
#include "LampState.h"
#include "Ring.h"
#include "LedViewer.h"

CRGB leds[LED_AMOUNT];

EventQueue eventQueue;

ButtonController buttonCtrl(eventQueue, BTN_PIN, false);

WiFiController wifiCtrl(eventQueue);

HttpController httpCtrl(HTTP_PORT);
WebSocketController wsCtrl(eventQueue, WS_PORT);

#if MATRIX_TYPE == 2
DoublePanelSnakeMatrix ledConfig(leds, WIDTH, HEIGHT);
#elif MATRIX_TYPE == 1
SnakeMatrix ledConfig(leds, WIDTH, HEIGHT);
#elif MATRIX_TYPE == 0
RowMatrix ledConfig(leds, WIDTH, HEIGHT);
#endif

uint8_t masterBrightness = 255;
Effect *effects[static_cast<uint8_t>(EffectId::COUNT)];
JustLampEffect justLamp(ledConfig, effSets[static_cast<uint8_t>(EffectId::JUST_LAMP)], masterBrightness);
RainbowHorizontalEffect rainbowH(ledConfig, effSets[static_cast<uint8_t>(EffectId::RAINBOW_HORIZONTAL)], masterBrightness);
RainbowVerticalEffect rainbowV(ledConfig, effSets[static_cast<uint8_t>(EffectId::RAINBOW_VERTICAL)], masterBrightness);

Ring ring(ledConfig);

EffectManager effManager(effects);

LedViewer ledViewer(effManager, ring);

LampCore core(eventQueue, effManager, wsCtrl, httpCtrl, wifiCtrl, ledViewer);

void showFiles(const String &path, uint8_t level = 0)
{
  Dir dir = LittleFS.openDir(path);

  while (dir.next())
  {
    // отступы
    for (uint8_t i = 0; i < level; i++)
      Serial.print("  ");

    String name = dir.fileName();

    if (dir.isDirectory())
    {
      Serial.printf("[DIR]  %s\n", name.c_str());
      showFiles(name, level + 1); // ВАЖНО: передаём name, а не path + name
    }
    else
    {
      Serial.printf("[FILE] %s (%d bytes)\n",
                    name.c_str(),
                    dir.fileSize());
    }
  }
}

void setup()
{

  Serial.begin(115200);
  delay(2000);

  effects[static_cast<uint8_t>(EffectId::JUST_LAMP)] = &justLamp;
  effects[static_cast<uint8_t>(EffectId::RAINBOW_HORIZONTAL)] = &rainbowH;
  effects[static_cast<uint8_t>(EffectId::RAINBOW_VERTICAL)] = &rainbowV;

  LittleFS.begin();

#ifdef USE_EEPROM
  lampStateFD.read();
  effSetsFD.read();
#endif

#ifdef DEBUG_SERIAL_LAMP
  showFiles("/");
#endif

  // LED setup
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COL_ORDER>(leds, LED_AMOUNT);
  FastLED.setBrightness(255);
  FastLED.clear(true);

  wifiCtrl.init();
  httpCtrl.init();
  wsCtrl.init();
  effManager.setEffect();
}

void loop()
{

  wifiCtrl.tick();

  httpCtrl.tick();
  wsCtrl.tick();

#ifdef USE_BTN
  buttonCtrl.tick();
#endif

  ledViewer.tick();
  yield();

#ifdef USE_EEPROM
  if (lampStateFD.tick() == FD_WRITE || effSetsFD.tick() == FD_WRITE)
    DEBUGLN("EEPROM UPDATED");
#endif

  core.tick();
}
