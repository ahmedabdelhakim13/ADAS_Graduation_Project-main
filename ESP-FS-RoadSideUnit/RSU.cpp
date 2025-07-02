#include "RSU.h"

// MAC address of the receiver (server) ESP32
static uint8_t broadcastAddress[] = {0xF0, 0x24, 0xF9, 0x59, 0x69, 0x88};

// Data structure instances
static struct_message myData;
static pedestrian_message message;

// Define GPIO 4 for Steering-Wheel interrupt, GPIO 2 for Pedestrian button
const int SWPin = 4;
const int PedestrianButtonPin = 2;

// Flags and debounce
bool SWFlag = false;
volatile bool SWISR_trigger = false;
bool isPressed = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 30; // 30ms debounce

// ISR for steering wheel
void IRAM_ATTR SWISR() {
  SWISR_trigger = true;
}

// Send status callback
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void RSU_init(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(SWPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWPin), SWISR, FALLING);

  pinMode(PedestrianButtonPin, INPUT);
}

void RSU_send_data(void) {
  // Handle steering wheel input
  if (SWISR_trigger) {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTime > debounceDelay) {
      isPressed = !isPressed;
      if (isPressed) {
        SWFlag = !SWFlag;
        if(SWFlag){
          // Send special message to notify ESP (main car) to start listening for steering data
          strcpy(message.text, "STEERING_START");
          esp_now_send(broadcastAddress, (uint8_t*)&message, sizeof(message));
          delay(200);   // Short delay to ensure message arrives before data
        }
        else {
          // Send special message to notify ESP (main car) to stop listening for steering data
          strcpy(message.text, "STEERING_END");
          esp_now_send(broadcastAddress, (uint8_t*)&message, sizeof(message));
          delay(200);   // Short delay to ensure message arrives before data
        }
      }
      lastDebounceTime = currentTime;
    }
    SWISR_trigger = false;
  }

  // Send steering + pedal data
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    if (SWFlag) {
      int commaIndex = data.indexOf(',');
      if (commaIndex > 0) {
        myData.steeringValue = data.substring(0, commaIndex).toFloat();
        myData.pedalValue = data.substring(commaIndex + 1).toFloat();

        esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));
      }
    }
    else {
      if (data.equalsIgnoreCase("Drowsy")) {
      strcpy(message.text, "Drowsy");  
      esp_now_send(broadcastAddress, (uint8_t*)&message, sizeof(message));
      }
    }
  }

  // Send pedestrian message
  if (digitalRead(PedestrianButtonPin) == HIGH) {
    strcpy(message.text, "Pedestrian wants to pass");
    esp_now_send(broadcastAddress, (uint8_t*)&message, sizeof(message));
    delay(1000);
  }
}
