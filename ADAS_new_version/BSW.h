#ifndef BSW_H
#define BSW_H

#include "Ultrasonic.h"

#define RU_TRIGGERPIN  PB4
#define RU_ECHOPIN     PB5
#define LU_TRIGGERPIN  PB6
#define LU_ECHOPIN     PB7
// #define Buzzer_PIN     PB12

void BSW_init();
void BSW_run();

#endif