#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include <Arduino.h>

void ultrasonic_init(unsigned char triggerPin, unsigned char echoPin);
unsigned long ultrasonic_Get_Distance(unsigned char triggerPin, unsigned char echoPin);

#endif
