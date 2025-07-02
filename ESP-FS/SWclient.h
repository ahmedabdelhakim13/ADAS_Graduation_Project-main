#ifndef SWclient_H
#define SWclient_H

#include <esp_now.h>
#include <WiFi.h>

// Structure to hold the data
typedef struct struct_message {
  float steeringValue;
  float pedalValue;
} struct_message;

extern bool SWFlag;
extern volatile bool SWISR_trigger;

void SWclient_init(void);
void SWclient_send_data(void);

#endif // SWclient_H
