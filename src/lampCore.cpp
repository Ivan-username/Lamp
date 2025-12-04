
#include "config.h"

#include "lampWiFi.h"
#include "effectsTicker.h"
#include "httpServer.h"
#include "webSocketServer.h"
#include "Matrix.h"
#include "lampButton.h"

// Main program
void setup()
{
  Serial.begin(115200);
  delay(2000);

  LittleFS.begin();

  FDstat_t stat = data.read();
  switch (stat)
  {
  case FD_FS_ERR:
    DEBUGLN("FS Error");
    break;
  case FD_FILE_ERR:
    DEBUGLN("Error");
    break;
  case FD_WRITE:
    DEBUGLN("Data Write");
    break;
  case FD_ADD:
    DEBUGLN("Data Add");
    break;
  case FD_READ:
    DEBUGLN("Data Read");
    break;
  default:
    break;
  }

  effects[0] = new Effect(renderer);        // Red
  effects[1] = new RainbowEffect(renderer); // Rainbow Vertical

  DEBUGLN("Data read:");
  DEBUGLN(config.wifiMode);
  DEBUGLN(config.STAssid);
  DEBUGLN(config.STApassword);

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
  iconAnimation(logoIcon, CRGB::Yellow, 1000); // "Logo"

  // WiFi setup
  DEBUGLN("Setting up WiFi: " + config.wifiMode);
  setupWiFi();

  // sevver setup
  setupHttpServer();
  setupWebSocketServer();
}

void loop()
{
  server.handleClient();
  webSocket.loop();
  effectsTick();
#if (USE_BTN == 1)
  buttonTick();
#endif
  ESP.wdtFeed(); // пнуть собаку
  yield();       // ещё раз пнуть собаку
}
