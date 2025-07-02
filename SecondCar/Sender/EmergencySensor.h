#ifndef EMERGENCY_SENSOR_H
#define EMERGENCY_SENSOR_H

#include <esp_now.h>
#include <WiFi.h>

typedef struct {
    float distance;
    bool emergency;
} SensorData;

void initEmergencySensor();
void sendEmergencyData();

#endif
