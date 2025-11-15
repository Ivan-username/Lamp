#pragma once

#include "config.h"

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
    
    <button id="btn"></button>
    
    <br>

    <li>
        <li id="lamp">lamp</li>
        <li id="fire">fire</li>
        <li id="lava">lava</li>
    </li>
    <br>
    
    <div>
        Brightness: <span id="brightnessValue"></span>
    </div>
    <input type="range" min="5" max="255" value="%BRIGHTNESS%" id="brightnessSlider">
    <br>
    <div>
        Scale: <span id="scaleValue"></span>
    </div>
    <input type="range" min="0" max="255" value="%SCALE%" id="scaleSlider">
    
    <div id="connectionStatus">
        Connecting...
    </div>

    <script>
        var socket = new WebSocket('ws://' + window.location.hostname + ':81/');
        const btn = document.getElementById('btn');

        const lamp = document.getElementById('lamp');
        const fire = document.getElementById('fire');
        const lava = document.getElementById('lava');


        const brightnessSlider = document.getElementById('brightnessSlider');
        const scaleSlider = document.getElementById('scaleSlider');

        const brightnessValue = document.getElementById('brightnessValue');
        const scaleValue = document.getElementById('scaleValue');

        const connectionStatus = document.getElementById('connectionStatus');

        //socket events
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
            console.log('event:' + event.data);
            if (event.data.startsWith('state:')) {
                const state = event.data.substring(6);
                updateButtonState(state === 'on');
            }
            else if (event.data.startsWith('brightness:')) {
                const brightness = event.data.substring(11);
                brightnessValue.innerText = brightness;
                brightnessSlider.value = brightness;
            }
            else if (event.data.startsWith('scale:')) {
                const scale = event.data.substring(6);
                scaleValue.innerText = scale;
                scaleSlider.value = scale;
            }
        };

        function updateButtonState(isOn) {
            if (isOn) {
                btn.innerText = 'TURN OFF';
            } else {
                btn.innerText = 'TURN ON';
            }
        }

        //elems events
        btn.onclick = function() {
            socket.send('state');
            console.log('state');
        }

        lamp.onclick = function() {
            socket.send('mode:0');
            console.log('mode:0');
        }

        fire.onclick = function() {
            socket.send('mode:1');
            console.log('mode:1');
        }

        lava.onclick = function() {
            socket.send('mode:2');
            console.log('mode:2');
        }

        brightnessSlider.oninput = function() {
            brightnessValue.innerText = this.value;
            socket.send('brightness:' + this.value);
            console.log('brightness:' + this.value);
        }

        scaleSlider.oninput = function() {
            scaleValue.innerText = this.value;
            socket.send('scale:' + this.value);
            console.log('scale:' + this.value);
        }
    </script>
</body>
</html>
)rawliteral";

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
        webSocket.sendTXT(num, isOn ? "state:on" : "state:off");
        webSocket.sendTXT(num, ("brightness:" + String(modes[currentModeID].brightness)).c_str());
        webSocket.sendTXT(num, ("scale:" + String(modes[currentModeID].scale)).c_str());
    }
    break;
    case WStype_TEXT:
    {
        // Обработка команд
        if (String((char *)payload).startsWith("state"))
        {
            isOn = !isOn;
            effectSlowStart = true;
            Serial.println(isOn);
        }
        else if (String((char *)payload).startsWith("mode:"))
        {
            String val = String((char *)payload).substring(5);
            currentModeID = val.toInt();
            effectSlowStart = true;
            webSocket.sendTXT(num, ("brightness:" + String(modes[currentModeID].brightness)).c_str());
            webSocket.sendTXT(num, ("scale:" + String(modes[currentModeID].scale)).c_str());
            Serial.println(val);
        }
        else if (String((char *)payload).startsWith("brightness:"))
        {
            String val = String((char *)payload).substring(11);
            modes[currentModeID].brightness = val.toInt();
            Serial.println(val);
        }
        else if (String((char *)payload).startsWith("scale:"))
        {
            String val = String((char *)payload).substring(6);
            modes[currentModeID].scale = val.toInt();
            Serial.println(val);
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
