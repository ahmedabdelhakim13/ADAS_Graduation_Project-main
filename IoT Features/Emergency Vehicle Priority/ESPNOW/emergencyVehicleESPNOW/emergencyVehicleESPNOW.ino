#include <esp_now.h>
#include <WiFi.h>

// Ultrasonic Sensor Pins
#define TRIG_PIN 5
#define ECHO_PIN 18

// Define the MAC address of the receiving ESP32
uint8_t receiverMAC[] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF}; // Replace with the actual MAC address of RV_ESP32

typedef struct {
    float distance;
    bool emergency;
} SensorData;

SensorData dataToSend;

void onSent(const uint8_t *macAddr, esp_now_send_status_t status) {
    Serial.print("Message Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void setup() {
    Serial.begin(115200);

    // Initialize WiFi in STA mode (required for ESP-NOW)
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed!");
        return;
    }
    esp_now_register_send_cb(onSent);

    // Add peer (receiver)
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, receiverMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    // Ultrasonic sensor setup
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
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

    // Prepare data
    dataToSend.distance = distance;
    dataToSend.emergency = (distance < 30);

    // Send data via ESP-NOW
    esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));

    delay(1000);
}
