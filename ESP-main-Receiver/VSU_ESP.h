#ifndef VSU_ESP_H
#define VSU_ESP_H

#include <esp_now.h>
#include <WiFi.h>

// Data from steering wheel
typedef struct {
    float steeringValue;
    float pedalValue;
} struct_message;

// General text message
typedef struct {
    char text[32];
} Message;

// Sensor data (e.g. from ultrasonic + emergency vehicle)
typedef struct {
    float distance;
    bool emergency;
} SensorData;

// Global instance for steering wheel data
// extern struct_message steeringData;

// Function declarations
void VSU_ESP_init(void);

#endif // VSU_ESP_H
