
#include "config.h"

#include "lampWiFi.h"
#include "effectsTicker.h"
#include "lampServer.h"

// Main program
void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1); // Turn off the LED

  // LED setup
  ledsSetup();
  modes[1].scale = 0; // костыль для правильной инициализации эффекта огня

  // WiFi setup
  setupWiFi();

  // sevver setup
  setupServer();
}

void loop()
{

  serverTick();

  effectsTick();
}
