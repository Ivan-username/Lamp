#pragma once

#include "config.h"

String getHTML()
{
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Lamp</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="style.css">
    <style>
    :root {
    /* Цвета и основные переменные */
    --bg: #0b0b0d;
    --card-bg: rgba(30, 30, 34, 0.6);
    --panel-bg: rgba(20, 20, 23, 0.45);
    --panel-border: rgba(255, 255, 255, 0.06);
    --accent: #3dd6a0;
    --muted: rgba(255, 255, 255, 0.65);
    --glass-blur: 10px;
    --radius: 18px;
    --panel-radius: 14px;
    --shadow: 0 6px 18px rgba(0, 0, 0, 0.6);
    --transition: 320ms cubic-bezier(0.22, 0.9, 0.32, 1);
    --panel-gap: 12px;
    --max-card-width: 420px; /* desktop visual size similar to mobile */
    --max-card-height: 780px;
}

* {
    box-sizing: border-box;
}
/* Page background */
html {
    height: auto;
}

/* Центрирование (desktop) */
body {
    margin: 0;
    padding: 20px;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
        "Helvetica Neue", Arial;
    min-height: 100vh; /* минимум на высоту окна */
    background: linear-gradient(180deg, var(--bg) 0%, #5c5c5c 80%);
    background-repeat: no-repeat;
    background-size: cover; /* растягиваем градиент на весь body */
    display: flex;
    flex-direction: column;
    align-items: center;
    overflow-y: auto;
}
/* Title */
h1 {
    margin: 10rem 0 30px 0;
    font-size: 3rem;
    color: #fff;
    letter-spacing: 0.2px;
    font-weight: 600;
}

hr {
    width: 90%;
    height: 1px;
    padding: 0;
    margin: 0;
    border: none;
    background-color: #5d5d5d;
}

/* Большая карточка settings */
.settings {
    width: 90%;
    /* height:100%; */
    max-width: var(--max-card-width);
    /* max-height:var(--max-card-height); */
    /* box-sizing:border-box; */
    background: var(--card-bg);
    border-radius: 15px;
    /* padding: 0 20px; */
    margin-bottom: 2rem;
    display: flex;
    flex-direction: column;
    align-items: center;
    backdrop-filter: blur(var(--glass-blur)) saturate(1.1);
    -webkit-backdrop-filter: blur(var(--glass-blur));
    box-shadow: var(--shadow);
    border: 1px solid rgba(255, 255, 255, 0.03);
    overflow: hidden;
}

.settings > div {
    width: 100%;
    min-height: 2rem;
    /* background-color: #ff80004f; */
}

.modes {
    display: flex;
    flex-flow: row wrap;
    justify-content: center;
    /* gap:12px; */
    padding: 16px 0;
}
.mode {
    margin: 0 2px;
    padding: 12px 16px;
    border-radius: 10px;
    border-color: #fff;
    border-style: dashed;
    color: var(--muted);
}

/* Sliders */
span {
    color: var(--muted);
    font-size: 1.2rem;
    padding: 0;
    margin: 0;
}

.slider-container {
    flex-direction: column;
    align-items: center;
    padding: 0.5rem 1rem;
}
.slider-info {
    display: flex;
    flex-flow: row nowrap;
    justify-content: space-between;
    width: 100%;
    padding-bottom: 0.5rem;
    color: var(--muted);
}

.slider {
    /* -webkit-appearance: none; */
    width: 100%;
    padding: 0;
    background: rgba(255, 255, 255, 0.2);
    outline: none;
    opacity: 0.9;
    -webkit-transition: 0.2s;
    transition: opacity 0.2s;
}

.div-button {
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: center;
    min-height: 2rem;
    gap: 60%;
    padding: 2px 0 4px 0;
    width: 100%;
}

.div-button:hover {
    cursor: pointer;
    opacity: 0.8;
}
.div-button:active {
    background-color: rgba(255, 255, 255, 0.1);
}

/* options */

.options {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
}

.options > div {
    width: 100%;
    min-height: 2rem;
    /* background-color: #ff80004f; */
}

.wifi-sta,
.wifi-ap {
    display: flex;
    flex-direction: column;
    align-items: center;
}

input[type="text"] {
    padding: 0.3rem;
    margin-top: 4px;
    font-size: 1rem;
    border-radius: 0.5rem;
    border: none;
    background-color: #1e1e1e;
    color: var(--muted);
}

.sta-container {
    width: 100%;
    padding: 0 1rem;
}

.sta-name,
.sta-password {
    display: flex;
    flex-flow: row nowrap;
    justify-content: space-between;
    align-items: center;
}

    </style>
</head>
<body>

    <h1>Lamp</h1>
    
    <div class="settings" id="settings">

        <div class="connectionStatus">

        </div>
<hr>
        <div class="modes" id="modes">
            <div class="mode" id="lamp">lamp</div>
            <div class="mode" id="fire">fire</div>
            <div class="mode" id="lava">lava</div>
        </div>

        <hr>

            <div class="slider-container">
                <div class="slider-info">
                    <span class="slider-name" id="slider-name">Brightness:</span>
                    <span id="brightness-value"></span>
                </div>
                <input type="range" min="1" max="255" id="slider-brightness" class="slider">
            </div>
<hr>
            <div class="slider-container">
                <div class="slider-info">
                    <span class="slider-name" id="slider-name">Scale:</span>
                    <span id="scale-value"></span>
                </div>
                <input type="range" min="1" max="255" id="slider-scale" class="slider">
            </div>
<hr>
            <div class="slider-container">
                <div class="slider-info">
                    <span class="slider-name" id="slider-name">Speed:</span>
                    <span id="speed-value"></span>
                </div>
                <input type="range" min="1" max="50" id="slider-speed" class="slider">
            </div>

        <hr>

        <div class="div-button" id="onoff-button">
            <span class="onoff-text" id="onoff-text">Switch
        </div>
    </div>

    <div class="settings">
        
        <div class="div-button" id="options-button">
            <span class="options-text" id="options-text">Options</span>
        </div>

        <div id="options" class="options">
            <hr>
            <div class="wifi-sta" id="wifi-sta">
                <div class="sta-container">
                    <div class="sta-name">
                        <span>Name:</span>
                        <input class="sta-input" type="text" required placeholder="..." id="wifiName">
                    </div>
                    <div class="sta-password">
                        <span>Pass:</span>
                        <input class="sta-input" type="text" required password placeholder="..." id="wifiPassword">
                    </div>
                </div>
                <div class="div-button" id="sta-button">
                    <span class="sta-text" id="sta-text">Set new WiFi</span>
                </div>
            </div>
            <hr>
            <div class="wifi-ap" id="wifi-ap">
                <div class="div-button" id="ap-button">
                    <span class="ap-text" id="ap-text">Switch to AP Mode</span>
                </div>
            </div>
        </div>

    </div>
    <script>
    document.addEventListener("DOMContentLoaded", () => {
    const optionsButton = document.getElementById("options-button");
    const optionsMenu = document.getElementById("options");

    // изначально скрываем меню (если нужно)
    optionsMenu.style.display = "none";

    optionsButton.addEventListener("click", () => {
        if (optionsMenu.style.display === "none") {
            optionsMenu.style.display = "flex"; // показать
        } else {
            optionsMenu.style.display = "none"; // скрыть
        }
    });

    // WebSocket
    var socket = new WebSocket("ws://" + window.location.hostname + ":81/");
    const onoff = document.getElementById("onoff-button");
    const onoffText = document.getElementById("onoff-text");

    const modes = document.getElementById("modes");

    const brightnessSlider = document.getElementById("slider-brightness");
    const scaleSlider = document.getElementById("slider-scale");
    const speedSlider = document.getElementById("slider-speed");

    const brightnessValue = document.getElementById("brightness-value");
    const scaleValue = document.getElementById("scale-value");
    const speedValue = document.getElementById("speed-value");

    const wifiName = document.getElementById("wifiName");
    const wifiPassword = document.getElementById("wifiPassword");
    const setupSTA = document.getElementById("sta-button");

    const setupAP = document.getElementById("ap-button");

    const status = document.getElementById("connectionStatus");
    var connectionStatus = true;

    //socket events
    socket.onopen = () => {
        status.textContent = "Connected";
    };
    socket.onclose = () => {
        status.textContent = "Disconnected";
    };
    socket.onerror = () => {
        status.textContent = "Connection error";
    };

    socket.onmessage = function (event) {
        console.log("event:" + event.data);
        if (event.data.startsWith("state:")) {
            const state = event.data.substring(6);
        } else if (event.data.startsWith("brightness:")) {
            const brightness = event.data.substring(11);
            brightnessValue.innerText = brightness;
            brightnessSlider.value = brightness;
        } else if (event.data.startsWith("scale:")) {
            const scale = event.data.substring(6);
            scaleValue.innerText = scale;
            scaleSlider.value = scale;
        } else if (event.data.startsWith("speed:")) {
            const speed = event.data.substring(6);
            speedValue.innerText = speed;
            speedSlider.value = speed;
        }
    };

    //elems events
    onoff.onclick = function () {
        if (connectionStatus) {
            socket.send("state");
            console.log("state");
        } else {
            location.reload(true);
        }
    };

    modes.onclick = function (event) {
        if (event.target.tagName === "DIV") {
            const modeIndex = Array.from(modes.children).indexOf(event.target);
            socket.send("mode:" + modeIndex);
            console.log("mode:" + modeIndex);
        }
    };

    setupSTA.onclick = function () {
        const ssid = wifiName.value;
        const password = wifiPassword.value;
        socket.send("wifi:" + ssid + "," + password);
        console.log("wifi:" + ssid + "," + password);
    };

    // sliders
    setupSlider(brightnessSlider, brightnessValue, "brightness");
    setupSlider(scaleSlider, scaleValue, "scale");
    setupSlider(speedSlider, speedValue, "speed");

    function sendSliderValue(command, value) {
        socket.send(command + ":" + value);
        console.log(command + ":" + value);
    }

    function setupSlider(slider, display, command) {
        slider.addEventListener(
            "input",
            throttle(function () {
                const value = slider.value;
                display.innerText = value;
                sendSliderValue(command, value);
            }, 50)
        ); // задержка 50 мс
    }

    // delay
    function throttle(func, delay) {
        let lastCall = 0;
        return function (...args) {
            const now = Date.now();
            if (now - lastCall >= delay) {
                lastCall = now;
                func.apply(this, args);
            }
        };
    }
});

</script>
</body>
</html>
)rawliteral";

    return html;
}