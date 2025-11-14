#pragma once

#include "config.h"

// Handlers
void handleRoot()
{
    String page = " <!DOCTYPE html> "
                  "<html lang=\"ru\"> "
                  "<head>"
                  "<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">"
                  "<title>HTML ESP8266</title>"
                  "</head>"
                  "<body> "
                  "<a href=\"/switch\">switch</a>"
                  "</body> "
                  "</html> ";
    server.send(200, "text/html", page);
}

void handleSwitch()
{

    if (currentModeID == MODE_AMOUNT - 1)
        currentModeID = 0;
    else
        currentModeID += 1;

    effectSlowStart = true;

    Serial.print("Current mode ID: ");
    Serial.println(currentModeID);
    // Toggle the LED state here !!!
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", ""); // Redirect to root
}

// server setup
void setupServer()
{
    server.on("/", handleRoot);
    server.on("/switch", handleSwitch);

    server.begin(); // Start the server
}

// server tick
void serverTick()
{
    server.handleClient();
}