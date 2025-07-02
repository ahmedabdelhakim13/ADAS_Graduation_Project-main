#include "VSU_ESP.h"

static struct_message steeringData;
static Message messageData;
static SensorData sensorData;
static bool steeringModeActive = false;
char buffer[50];

void  OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  if (len == sizeof(struct_message)) {
    if (steeringModeActive){
      memcpy(&steeringData, incomingData, sizeof(steeringData));
      sprintf(buffer, "%.2f,%.2f", steeringData.steeringValue, steeringData.pedalValue);
      Serial2.println(buffer);
      Serial.print("Steering: ");
      Serial.println(steeringData.steeringValue);
      Serial.print("Pedal: ");
      Serial.println(steeringData.pedalValue);
    }
  } else if (len == sizeof(Message)) {
    memcpy(&messageData, incomingData, sizeof(messageData));
    Serial.print("Message received: ");
    Serial.println(messageData.text);
    // Check if it's a steering mode trigger
    if (strcmp(messageData.text, "STEERING_START") == 0) {
      steeringModeActive = true;
      // Send STEERING_MODE to the STM32 to start listening for steering data
      Serial2.println("STEERING_MODE");
      Serial.println("Steering mode activated: waiting for data...");
    } else if (strcmp(messageData.text, "STEERING_END") == 0) {
      steeringModeActive = false;
      // Send NORMAL_MODE to the STM32 to stop listening for steering data
      Serial2.println("NORMAL_MODE");
      Serial.println("Steering mode deactivated.");
    }
    else if(strcmp(messageData.text, "Pedestrian wants to pass") == 0)
    {
      Serial2.println("Pedestrian wants to pass");
    }
    else if(strcmp(messageData.text, "Drowsy") == 0) {
      // Drowsness Detected
      Serial2.println("Drowsy");
      Serial.println("Drowness Detected!");
    }
  } else if (len == sizeof(SensorData)) {
    memcpy(&sensorData, incomingData, sizeof(sensorData));
    Serial.print("Received Distance: ");
    Serial.print(sensorData.distance);
    Serial.println(" cm");
    if (sensorData.emergency) {
      Serial.println("Emergency vehicle approaching! Please clear the way.");
      Serial2.println("Emergency\n");
    }
  } else {
    Serial.println("Unknown data format received.");
  }
}

void VSU_ESP_init(void) {
  Serial.begin(115200);

  Serial2.begin(9600, SERIAL_8N1, 16, 17); // UART2: TX=17, RX=16

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
