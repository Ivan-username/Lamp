#pragma once

#include "config.h"
#include "LampWiFi.h"
class LampHttpServer
{
public:
    LampHttpServer(uint16_t port = 80)
        : _server(port)
    {
    }

    void initHttpServer()
    {
        _server.on("/", [this]()
                   { handleIndex(); });
        _server.on("/index.html", [this]()
                   { handleIndex(); });

        _server.on("/style.css", [this]()
                   { handlePageComponents(); });
        _server.on("/script.js", [this]()
                   { handlePageComponents(); });

        _server.on("/icons/ip.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/reload.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/brightness.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/scale.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/speed.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/onoff.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/options.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/sta.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/ap.svg", [this]()
                   { handlePageComponents(); });
        _server.on("/icons/reboot.svg", [this]()
                   { handlePageComponents(); });

        _server.begin();
    }

    void tick()
    {
        _server.handleClient();
    }

private:
    void handlePageComponents()
    {
        String path = _server.uri();

        File f = LittleFS.open(path, "r");
        if (!f)
        {
            _server.send(404, "text/plain", "Check:\nresources loaded correctly\nuris in html");
            return;
        }

        if (path.endsWith(".css"))
            _server.streamFile(f, "text/css");
        else if (path.endsWith(".js"))
            _server.streamFile(f, "application/javascript");
        else if (path.startsWith("/icons"))
            _server.streamFile(f, "image/svg+xml");
        else
            _server.send(404, "text/plain", "Unknown resource.");

        f.close();
    }

    void handleIndex()
    {
        File f = LittleFS.open("/index.html", "r");
        if (!f)
        {
            _server.send(404, "text/plain", "index.html not found");
            return;
        }

        _server.streamFile(f, "text/html");
        f.close();
    }

    ESP8266WebServer _server;
};