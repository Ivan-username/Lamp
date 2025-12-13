document.addEventListener("DOMContentLoaded", () => {
    const optionsBtn = document.getElementById("card-btn-options");
    const options = document.getElementById("options");

    options.style.display = "none";

    optionsBtn.addEventListener("click", () => {
        if (options.style.display === "none") {
            options.style.display = "flex";
        } else {
            options.style.display = "none";
        }
    });

    // WebSocket
    var socket = new WebSocket("ws://" + window.location.hostname + ":81/");

    const statusBar = document.getElementById("status-bar");

    const reloadBtn = document.getElementById("card-btn-reload");

    const ip = document.getElementById("ip");

    const effects = document.getElementById("effects");

    const brightnessSlider = document.getElementById("slider-brightness");
    const scaleSlider = document.getElementById("slider-scale");
    const speedSlider = document.getElementById("slider-speed");

    const onoffBtn = document.getElementById("card-btn-onoff");

    const staName = document.getElementById("sta-name");
    const staPassword = document.getElementById("sta-password");
    const setupSTABtn = document.getElementById("card-btn-sta");

    const setupAPBtn = document.getElementById("card-btn-ap");

    const rebootBtn = document.getElementById("card-btn-reboot");

    //socket events
    socket.onopen = () => {
        statusBar.style.backgroundColor = "green";
    };
    socket.onclose = () => {
        statusBar.style.backgroundColor = "red";
    };
    socket.onerror = () => {
        statusBar.style.backgroundColor = "red";
    };

    socket.onmessage = function (event) {
        statusBar.style.backgroundColor = "green";
        console.log("event:", event.data);

        let commands = event.data.split("|");

        commands.forEach((element) => {
            if (element.startsWith("IP:")) {
                ip.textContent = element.substring(3);
            } else if (element.startsWith("BRIGHT:")) {
                brightnessSlider.value = element.substring(7);
            } else if (element.startsWith("SCALE:")) {
                scaleSlider.value = element.substring(6);
            } else if (element.startsWith("SPEED:")) {
                speedSlider.value = element.substring(6);
            } else if (element.startsWith("SAVED_STA:")) {
                const data = element.substring(10);
                const [ssid, pass] = data.split(",");
                staName.placeholder = ssid || "";
                staPassword.placeholder = pass || "";
            }
        });
    };

    //elems events
    reloadBtn.onclick = function () {
        location.reload(true);
    };

    onoffBtn.onclick = function () {
        socket.send("POWER");
        console.log("POWER");
    };

    effects.onclick = function (event) {
        if (event.target.tagName === "DIV") {
            const effectIndex = Array.from(effects.children).indexOf(
                event.target
            );
            socket.send("EFFECT:" + effectIndex);
            console.log("EFFECT:" + effectIndex);
        }
    };

    setupSTABtn.onclick = function () {
        const ssid = staName.value;
        const password = staPassword.value;
        socket.send("sta:" + ssid + "," + password);
        console.log("sta:" + ssid + "," + password);
        statusBar.style.backgroundColor = "red";
    };

    setupAPBtn.onclick = function () {
        socket.send("ap");
        console.log("ap");
        statusBar.style.backgroundColor = "red";
    };

    rebootBtn.onclick = function () {
        socket.send("reboot");
        console.log("reboot");
        statusBar.style.backgroundColor = "red";
    };

    // sliders
    setupSlider(brightnessSlider, "BRIGHT");
    setupSlider(scaleSlider, "SCALE");
    setupSlider(speedSlider, "SPEED");

    function sendSliderValue(command, value) {
        socket.send(command + ":" + value);
        console.log(command + ":" + value);
    }

    function setupSlider(slider, command) {
        slider.addEventListener(
            "input",
            throttle(function () {
                const value = slider.value;
                sendSliderValue(command, value);
            }, 50)
        );
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
