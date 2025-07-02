#include <esp_now.h>
#include <WiFi.h>

struct Message {
  char text[32];
};

typedef struct {
  float distance;
  bool emergency;
} SensorData;

Message message;
SensorData receivedData;

void onReceive(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  if (len == sizeof(Message)) {
    memcpy(&message, incomingData, sizeof(message));
    Serial.print("Message received: ");
    Serial.println(message.text);
  } else if (len == sizeof(SensorData)) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    Serial.print("Received Distance: ");
    Serial.print(receivedData.distance);
    Serial.println(" cm");
    
    if (receivedData.emergency) {
      Serial.println("Emergency vehicle approaching! Please clear the way.");
    }
  }
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
