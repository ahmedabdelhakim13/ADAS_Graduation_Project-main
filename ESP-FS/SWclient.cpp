#include "SWclient.h"

// MAC address of the receiver (server) ESP32
static uint8_t broadcastAddress[] = {0xF0, 0x24, 0xF9, 0x59, 0x69, 0x88};

// Data structure instance
static struct_message myData;

// Define GPIO 4 to be used as interrupt trigger for Steering-Wheel feature
const int SWPin = 4;

// Steering-Wheel Flag
bool SWFlag = false;

// Handle debouncing
volatile bool SWISR_trigger = false;
bool isPressed = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 30; // 30ms for solid debounce

// Interrupt Service Routine (ISR)
void IRAM_ATTR SWISR() {
  SWISR_trigger = true;
}


void SWclient_init(void) {
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    // Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    // Serial.println("Failed to add peer");
    return;
  }

  // Define SWPin to be input (active low)
  pinMode(SWPin, INPUT_PULLUP);

  // Attach interrupt to SWPin on falling edge
  attachInterrupt(digitalPinToInterrupt(SWPin), SWISR, FALLING);
}

void SWclient_send_data(void) {
  // Check if the ISR is executed
  if (SWISR_trigger) {
    unsigned long currentTime = millis();
    
    // Only process if debounce time has passed
    if (currentTime - lastDebounceTime > debounceDelay) {
      isPressed = !isPressed;

      if (isPressed) {
        SWFlag = !SWFlag;
        Serial.print("Sending mode: ");
        Serial.println(SWFlag ? "ON" : "OFF");
      }

      lastDebounceTime = currentTime;
    }

    SWISR_trigger = false;
  }

  // Check if SWFlag is true
  if(SWFlag == true)
  {
    // Check if there is data on the serial to send it
    if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();

    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      myData.steeringValue = data.substring(0, commaIndex).toFloat();
      myData.pedalValue = data.substring(commaIndex + 1).toFloat();

      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));
      } 
    }
  }
}