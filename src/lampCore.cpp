
#include "config.h"

#include "lampWiFi.h"
#include "effectsTicker.h"
#include "httpServer.h"
#include "webSocketServer.h"
#include "lampLedUtils.h"
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
  ledsSetup();
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
  buttonTick();
  ESP.wdtFeed(); // пнуть собаку
  yield();       // ещё раз пнуть собаку
}
