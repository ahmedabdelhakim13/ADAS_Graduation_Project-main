#include "SWserver.h"

// Data structure instance to hold incoming data
static struct_message incomingData;

// Define the button pin for toggling receiving mode
static const int SWPin = 4;  // GPIO pin for mode toggle

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

// Callback function for when data is received
static void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
  if (SWFlag) {
    // Copy the received data into the local structure
    memcpy(&incomingData, data, sizeof(incomingData));

    // Print received data
    Serial.print("Steering: ");
    Serial.println(incomingData.steeringValue);
    Serial.print("Pedal: ");
    Serial.println(incomingData.pedalValue);
  }
}

void SWserver_init(void) {
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback
  esp_now_register_recv_cb(OnDataRecv);

  // Initialize the mode toggle pin
  pinMode(SWPin, INPUT_PULLUP);

  // Attach interrupt to SWPin on falling edge
  attachInterrupt(digitalPinToInterrupt(SWPin), SWISR, FALLING);
}

void SWserver_receive_data(void) {
  // Check if the ISR is executed
  if (SWISR_trigger) {
    unsigned long currentTime = millis();
    
    // Only process if debounce time has passed
    if (currentTime - lastDebounceTime > debounceDelay) {
      isPressed = !isPressed;

      if (isPressed) {
        SWFlag = !SWFlag;
        Serial.print("Receiving mode: ");
        Serial.println(SWFlag ? "ON" : "OFF");
      }

      lastDebounceTime = currentTime;
    }

    SWISR_trigger = false;
  }
}

