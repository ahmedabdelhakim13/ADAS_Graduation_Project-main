#include "EmergencySensor.h"
#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

MPU6050 imu;
TinyGPSPlus gps;
HardwareSerial gpsSerial(1); 


esp_now_peer_info_t peerInfo;

uint8_t receiverMAC[] = {0xF0, 0x24, 0xF9, 0x59, 0x69, 0x88};  // reciever ESP32 MAC Address: 8C:4F:00:28:96:6C


typedef struct {
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
  float latitude, longitude;
} SensorDataMain;

SensorDataMain dataToSend;

void onDataSent(const uint8_t *mac, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  imu.initialize();
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  WiFi.mode(WIFI_STA);

  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  
  esp_now_register_send_cb(onDataSent);

  
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  if (gps.location.isValid()) {
    dataToSend.latitude = gps.location.lat();
    dataToSend.longitude = gps.location.lng();
  }

  
  int16_t ax, ay, az, gx, gy, gz;
  imu.getAcceleration(&ax, &ay, &az);
  imu.getRotation(&gx, &gy, &gz);
  
  dataToSend.accelX = ax / 16384.0;
  dataToSend.accelY = ay / 16384.0;
  dataToSend.accelZ = az / 16384.0;
  dataToSend.gyroX = gx / 131.0;
  dataToSend.gyroY = gy / 131.0;
  dataToSend.gyroZ = gz / 131.0;

  
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));
  
  if (result == ESP_OK) {
    Serial.println("Sent Data Successfully");
  } else {
    Serial.println("Send Failed");
  }

  delay(2000);
}
