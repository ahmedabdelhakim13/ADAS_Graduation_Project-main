#include <esp_now.h>
#include <WiFi.h>

typedef struct {
    float distance;
    bool emergency;
} SensorData;

SensorData receivedData;

// Callback function to handle received data
void onReceive(const uint8_t *macAddr, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));

    Serial.print("Received Distance: ");
    Serial.print(receivedData.distance);
    Serial.println(" cm");

    if (receivedData.emergency) {
        Serial.println("Emergency vehicle approaching! Please clear the way.");
    }
}

void setup() {
    Serial.begin(115200);

    // Initialize WiFi in STA mode
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed!");
        return;
    }

    // Register receive callback function
    esp_now_register_recv_cb(onReceive);
}

void loop() {
    // Nothing needed in loop() since data is received via callback
}
