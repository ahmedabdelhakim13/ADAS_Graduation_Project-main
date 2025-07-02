#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include <Servo.h>  // Arduino Servo library

typedef struct {
    Servo s;
    uint8_t pin;
    int16_t angle;
} ServoMotor;

void ServoMotor_init(ServoMotor* motor, uint8_t pin);
void ServoMotor_attach(ServoMotor* motor);
void ServoMotor_setAngle(ServoMotor* motor, int16_t angle);
int16_t ServoMotor_getAngle(ServoMotor* motor);

#endif