#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

MPU6050 imu;
TinyGPSPlus gps;
HardwareSerial gpsSerial(1); 

typedef struct {
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
  float latitude, longitude;
} CarData;

CarData car1Data, car2Data;  

void onDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&car2Data, incomingData, sizeof(car2Data));
  
  Serial.println("\n Car 2 (Received Data):");
  Serial.printf("Location: %.6f, %.6f\n", car2Data.latitude, car2Data.longitude);
  Serial.printf("Accel: X=%.2f, Y=%.2f, Z=%.2f\n", car2Data.accelX, car2Data.accelY, car2Data.accelZ);
  Serial.printf("Gyro: X=%.2f, Y=%.2f, Z=%.2f\n", car2Data.gyroX, car2Data.gyroY, car2Data.gyroZ);
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

  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  if (gps.location.isValid()) {
    car1Data.latitude = gps.location.lat();
    car1Data.longitude = gps.location.lng();
  }

  int16_t ax, ay, az, gx, gy, gz;
  imu.getAcceleration(&ax, &ay, &az);
  imu.getRotation(&gx, &gy, &gz);

  car1Data.accelX = ax / 16384.0;
  car1Data.accelY = ay / 16384.0;
  car1Data.accelZ = az / 16384.0;
  car1Data.gyroX = gx / 131.0;
  car1Data.gyroY = gy / 131.0;
  car1Data.gyroZ = gz / 131.0;

  Serial.println("\n Car 1 Data:");
  Serial.printf("Location: %.6f, %.6f\n", car1Data.latitude, car1Data.longitude);
  Serial.printf("Accel: X=%.2f, Y=%.2f, Z=%.2f\n", car1Data.accelX, car1Data.accelY, car1Data.accelZ);
  Serial.printf("Gyro: X=%.2f, Y=%.2f, Z=%.2f\n", car1Data.gyroX, car1Data.gyroY, car1Data.gyroZ);

  delay(2000);
}
