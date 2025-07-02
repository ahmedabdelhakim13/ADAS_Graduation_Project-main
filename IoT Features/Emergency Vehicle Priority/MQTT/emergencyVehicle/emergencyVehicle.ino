#include "WiFi_MQTT.h"

// Ultrasonic Sensor Pins
#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
    Serial.begin(115200);

    connecttoWiFi();
    connecttoMQTT("EV_ESP32", false);
    // Ultrasonic Setup
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    if (!client.connected()) {
        reconnect("EV_ESP32", false);
    }
    client.loop();

    // Measure distance
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2; // Convert to cm

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // If another vehicle is detected within 30 cm, send alert
    if (distance < 30) {
        Serial.println("Obstacle detected! Sending emergency signal...");
        client.publish(topic, "Emergency vehicle approaching! Please clear the way.");
        delay(5000); // Avoid spamming messages
    }

    delay(1000);
}

