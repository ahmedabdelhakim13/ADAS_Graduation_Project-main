#include "WiFi_MQTT.h"

void setup() {
    Serial.begin(115200);
    // MQTT Setup
    connecttoWiFi();
    connecttoMQTT("RV_ESP32", true);
}

void loop() {
    if (!client.connected()) {
        reconnect("RV_ESP32", true);
    }
    client.loop();
}
