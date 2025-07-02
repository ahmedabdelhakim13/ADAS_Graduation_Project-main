#ifndef SWserver_H
#define SWserver_H

#include <esp_now.h>
#include <WiFi.h>

// Structure to hold the data
typedef struct struct_message {
    float steeringValue;
    float pedalValue;
} struct_message;

extern bool SWFlag;
extern volatile bool SWISR_trigger;

// Function declarations
void SWserver_init(void);
void SWserver_receive_data(void);

#endif // SWserver_H