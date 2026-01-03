#pragma once

#include <ESP8266WebServer.h>

class HttpController
{
public:
    HttpController(uint16_t port = 80)
        : _server(port)
    {
    }

    void init()
    {
        setupRoutes();
        _server.begin();
    }
    void drop()
    {
        _server.close();
    }

    void tick()
    {
        _server.handleClient();
    }

private:
    void setupRoutes()
    {
        _server.on("/", [this]()
                   { handleIndex(); });
        _server.on("/index.html", [this]()
                   { handleIndex(); });

        const char *files[] = {
            "/style.css", "/script.js",
            "/icons/ip.svg", "/icons/reload.svg",
            "/icons/brightness.svg", "/icons/scale.svg",
            "/icons/speed.svg", "/icons/onoff.svg",
            "/icons/options.svg", "/icons/sta.svg",
            "/icons/ap.svg", "/icons/reboot.svg"};

        for (const char *f : files)
            _server.on(f, [this]()
                       { handlePageComponents(); });
    }

    void
    handlePageComponents()
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