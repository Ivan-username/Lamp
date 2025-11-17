
#include "config.h"

#include "lampWiFi.h"
#include "effectsTicker.h"
#include "lampServer.h"

// Main program
void setup()
{
  Serial.begin(115200);
  delay(1000);

  LittleFS.begin();

  FDstat_t stat = data.read();
  switch (stat)
  {
  case FD_FS_ERR:
    Serial.println("FS Error");
    break;
  case FD_FILE_ERR:
    Serial.println("Error");
    break;
  case FD_WRITE:
    Serial.println("Data Write");
    break;
  case FD_ADD:
    Serial.println("Data Add");
    break;
  case FD_READ:
    Serial.println("Data Read");
    break;
  default:
    break;
  }

  Serial.println("Data read:");
  Serial.println(config.wifiMode);
  Serial.println(config.STAssid);
  Serial.println(config.STApassword);

  // LED setup
  ledsSetup();
  modes[1].scale = 0; // костыль для правильной инициализации эффекта огня

  // WiFi setup
  Serial.println("Setting up WiFi: " + config.wifiMode);
  setupWiFi();

  // sevver setup
  setupHttpServer();
  setupWebsocketServer();
}

void loop()
{
  server.handleClient();
  webSocket.loop();

  effectsTick();
}
