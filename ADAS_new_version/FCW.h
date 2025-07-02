#ifndef FCW_H
#define FCW_H

#include "Ultrasonic.h"
#include "Motors.h"

#define FU_TRIGGERPIN  PB8
#define FU_ECHOPIN     PB9

extern short maxSpeed;
extern short avgSpeed;
extern short minSpeed;
extern short currentSpeed;

void FCW_init();
void FCW_run();

#endif