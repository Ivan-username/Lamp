#pragma once

#include "config.h"

boolean ledState = false;
byte ledBrightness = 100;

String getHTML()
{
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>LED Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
    <h1>LED Control</h1>
    
    <div>
        Status: <span id="statusText">%STATE%</span>
    </div>
    
    <button id="btn">%BUTTON_TEXT%</button>
    
    <br>
    
    <div>
        Brightness: <span id="brightnessValue">%BRIGHTNESS%</span>
    </div>
    <input type="range" min="5" max="255" value="%BRIGHTNESS%" id="brightnessSlider">
    
    <div id="connectionStatus">
        Connecting...
    </div>

    <script>
        var socket = new WebSocket('ws://' + window.location.hostname + ':81/');
        const btn = document.getElementById('btn');
        const slider = document.getElementById('brightnessSlider');
        const statusText = document.getElementById('statusText');
        const brightnessValue = document.getElementById('brightnessValue');
        const connectionStatus = document.getElementById('connectionStatus');

        socket.onopen = function(e) {
            connectionStatus.innerHTML = 'Connected';
        };

        socket.onclose = function(e) {
            connectionStatus.innerHTML = 'Disconnected';
        };

        socket.onerror = function(e) {
            connectionStatus.innerHTML = 'Connection error';
        };

        socket.onmessage = function(event) {
            if (event.data.startsWith('state:')) {
                const state = event.data.substring(6);
                updateButtonState(state === 'on');
            }
            else if (event.data.startsWith('brightness:')) {
                const brightness = event.data.substring(11);
                brightnessValue.innerText = brightness;
                slider.value = brightness;
            }
        };

        function updateButtonState(isOn) {
            if (isOn) {
                btn.innerText = 'TURN OFF';
                statusText.innerText = 'ON';
            } else {
                btn.innerText = 'TURN ON';
                statusText.innerText = 'OFF';
            }
        }

        btn.onclick = function() {
            socket.send('switch');
        }

        let brightnessTimeout;
        slider.oninput = function() {
            brightnessValue.innerText = this.value;
            clearTimeout(brightnessTimeout);
            brightnessTimeout = setTimeout(() => {
                socket.send('brightness:' + this.value);
            }, 100);
        }
    </script>
</body>
</html>
)rawliteral";
    html.replace("%STATE%", ledState ? "ВКЛ" : "ВЫКЛ");
    html.replace("%BRIGHTNESS%", String(ledBrightness));
    return html;
}

void handlerRoot()
{
    server.send(200, "text/html", getHTML());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
        Serial.println(" Disconnected!");
        break;
    }
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, ledState ? "state:on" : "state:off");
        // webSocket.sendTXT(num, "brightness:" + String(ledBrightness));
    }
    break;
    case WStype_TEXT:
    {
        Serial.printf("[%u] Received: %s\n", num, payload);

        // Обработка команд
        if (strcmp((char *)payload, "switch") == 0)
        {

            if (currentModeID == MODE_AMOUNT - 1)
                currentModeID = 0;
            else
                currentModeID += 1;
            effectSlowStart = true;
            // Можно отправить обратно новое состояние
            webSocket.sendTXT(num, ledState ? "state:on" : "state:off");
        }
        else if (String((char *)payload).startsWith("brightness:"))
        {
            String val = String((char *)payload).substring(11);
            modes[currentModeID].brightness = val.toInt();
        }
        break;
    }
    case WStype_BIN:
    {
        Serial.printf("[%u] Received binary data of length: %u\n", num, length);
        break;
    }
    case WStype_ERROR:
    {
        Serial.printf("[%u] Error occurred\n", num);
        break;
    }
    }
}

void setupHttpServer()
{
    server.on("/", handlerRoot);
    server.begin();
    Serial.println("HTTP server started.");
}

void setupWebsocketServer()
{
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("WebSocket server started.");
}
