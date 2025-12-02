#pragma once

#include "config.h"
#include "lampWiFi.h"
#include "lampLedUtils.h"

void handleIndex()
{
    DEBUGLN("Index was asked");

    File f = LittleFS.open("index.html", "r");
    if (!f)
    {
        server.send(404, "text/plain", "Check:\nresources was loaded correctly\nuris in html");
        return;
    }
    server.streamFile(f, "text/html");
    f.close();
}

void handlePageComponents()
{
    String path = server.uri();
    DEBUGLN("Uri recived:" + path);

    File f = LittleFS.open(path, "r");
    if (!f)
    {
        server.send(404, "text/plain", "Check:\nresources was loaded correctly\nuris in html");
        return;
    }
    if (path.startsWith("/style.css"))
        server.streamFile(f, "text/css");
    else if (path.startsWith("/script.js"))
        server.streamFile(f, "application/javascript");
    else if (path.startsWith("/icons"))
        server.streamFile(f, "image/svg+xml");
    else
        server.send(404, "text/plain", "Check:\nresources was loaded correctly\nuris in html");
    f.close();
}

void setupHttpServer()
{
    server.on("/", handleIndex);
    server.on("/index.html", handleIndex);

    server.on("/style.css", handlePageComponents);
    server.on("/script.js", handlePageComponents);

    server.on("/icons/ip.svg", handlePageComponents);
    server.on("/icons/reload.svg", handlePageComponents);
    server.on("/icons/brightness.svg", handlePageComponents);
    server.on("/icons/scale.svg", handlePageComponents);
    server.on("/icons/speed.svg", handlePageComponents);
    server.on("/icons/onoff.svg", handlePageComponents);
    server.on("/icons/options.svg", handlePageComponents);
    server.on("/icons/sta.svg", handlePageComponents);
    server.on("/icons/ap.svg", handlePageComponents);
    server.on("/icons/reboot.svg", handlePageComponents);

    server.begin();
    iconAnimation(&httpIcon[0][0], CRGB::White, INFO_SERV_ANIMATION);
    DEBUGLN("HTTP server started.");
}
