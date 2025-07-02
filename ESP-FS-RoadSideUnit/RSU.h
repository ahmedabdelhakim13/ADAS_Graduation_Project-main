#ifndef RSU_H
#define RSU_H

#include <esp_now.h>
#include <WiFi.h>

// Structure to hold the data
typedef struct struct_message {
  float steeringValue;
  float pedalValue;
} struct_message;

typedef struct {
  char text[32];
} pedestrian_message;

extern bool SWFlag;
extern volatile bool SWISR_trigger;

void RSU_init(void);
void RSU_send_data(void);

#endif // RSU_H
