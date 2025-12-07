
#include "config.h"

#include "LampWiFi.h"
#include "LampHttpServer.h"
#include "WebSocketController.h"
#include "Matrix.h"
#include "Renderer.h"
#include "Effect.h"
#include "EffectsManager.h"
#include "EventBus.h"
#include "LampCore.h"
#include "Button.h"
#include "ButtonController.h"

EventBus bus;
// effects
Effect *effects[EFFECTS_AMOUNT];
EffectsManager effectsManager(EFFECTS_AMOUNT, effects);

// core
LampCore core(bus, effectsManager);

// controllers
Button btn(BTN_PIN, false);
ButtonController btnCtrl(btn, bus);
WebSocketController wsCtrl(bus, 81);

// Objects
CRGB leds[LED_AMOUNT];

LampWiFi lampWiFi(
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

LampHttpServer lampHttpServer(80);

#if MATRIX_TYPE == 2
Matrix *matrix = new DoublePanelSnakeMatrix(WIDTH, HEIGHT, leds);
#elif MATRIX_TYPE == 1
Matrix *matrix = new SnakeMatrix(WIDTH, HEIGHT, leds);
#elif MATRIX_TYPE == 0
Matrix *matrix = new RowMatrix(WIDTH, HEIGHT, leds);
#endif

IRenderer *renderer = new MatrixRenderer(matrix);

// Main program
void setup()
{
  effects[0] = new Effect(renderer);        // Red
  effects[1] = new RainbowEffect(renderer); // Rainbow Vertical

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
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COL_ORDER>(matrix->_leds, LED_AMOUNT);
  FastLED.setBrightness(0);
  FastLED.clear(true);

  // Check led buffers pointers
  Serial.println((uint32_t)leds, HEX);
  Serial.println((uint32_t)matrix->_leds, HEX);

  lampWiFi.initWiFi();
  lampHttpServer.initHttpServer();

  wsCtrl.init();
  core.init();
}

void loop()
{
  btnCtrl.tick();
  yield();
  wsCtrl.tick();
  yield();
  effectsManager.tick();
  yield();
  lampHttpServer.tick();
  yield();
}
