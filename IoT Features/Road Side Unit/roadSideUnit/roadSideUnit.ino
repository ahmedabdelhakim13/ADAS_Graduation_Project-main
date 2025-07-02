#include <esp_now.h>
#include <WiFi.h>

uint8_t peerAddress[] = {0x24, 0x6F, 0x28, 0x12, 0x34, 0x56}; // Replace with receiver's MAC address

struct Message {
  char text[32];
};

Message message;

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if (digitalRead(2) == HIGH) { // Assuming button is connected to GPIO 2
    strcpy(message.text, "Pedestrian wants to pass");
    esp_now_send(peerAddress, (uint8_t *)&message, sizeof(message));
    delay(1000);
  }
}
