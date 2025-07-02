#ifndef APA_H
#define APA_H

#include "Ultrasonic.h"
#include "Motors.h"
#define REAR_TRIGGER_PIN PB12
#define REAR_ECHO_PIN    PB13

void APA_init();
void APA_run();

#endif