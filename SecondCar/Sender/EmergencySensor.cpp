#include "EmergencySensor.h"

// Ultrasonic Sensor Pins
#define TRIG_PIN 25
#define ECHO_PIN 26

// MAC address of the receiver
static uint8_t receiverMAC[] = {0xF0, 0x24, 0xF9, 0x59, 0x69, 0x88};

static SensorData dataToSend;

void onSent(const uint8_t *macAddr, esp_now_send_status_t status) {
    //Serial.print("Message Send Status: ");
    //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void initEmergencySensor() {
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed!");
        return;
    }

    esp_now_register_send_cb(onSent);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, receiverMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void sendEmergencyData() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2;

    //Serial.print("Distance: ");
    //Serial.print(distance);
    //Serial.println(" cm");

    if (distance < 30.0) {
        dataToSend.distance = distance;
        dataToSend.emergency = true;


        esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));
    }
}
