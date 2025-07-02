#ifndef ACC_H
#define ACC_H

#include "Ultrasonic.h"
#include "Motors.h"
#include "FCW.h"

#define SAFE_DISTANCE 50  
#define DECELERATION_RATE 10  
#define ACCELERATION_RATE 5 

void ACC_init();
void ACC_run();

#endif