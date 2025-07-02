#ifndef VSU_H
#define VSU_H

#include "Motors.h"

#define ServoPin PA0

extern bool flag;

typedef struct {
  float steeringValue;
  float pedalValue;
}steeringWheel;

struct Message {
  char text[32];
};

typedef struct {
  float distance;
  bool emergency;
} SensorData;

void VSU_init();   // Initialize UART or buffers
void VSU_run();    // Main processing function
bool isInSteeringMode();    // Check if we're in steering mode

#endif // VSU_H
