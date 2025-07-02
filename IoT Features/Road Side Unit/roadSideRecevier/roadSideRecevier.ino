#include <esp_now.h>
#include <WiFi.h>

struct Message {
  char text[32];
};

Message message;

void onReceive(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  Serial.print("Message received: ");
  Serial.println(message.text);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // Nothing to do here, waiting for incoming messages
}
