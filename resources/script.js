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
            }, 10)
        ); // задержка 10 мс
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
