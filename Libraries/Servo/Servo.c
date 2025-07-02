// servo.c
#include "servo.h"

void ServoMotor_init(ServoMotor* motor, uint8_t pin) {
    motor->pin = pin;
    motor->angle = 0;
}

void ServoMotor_attach(ServoMotor* motor) {
    motor->s.attach(motor->pin);
}

void ServoMotor_setAngle(ServoMotor* motor, int16_t angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    motor->angle = angle;
    motor->s.write(angle);
}

int16_t ServoMotor_getAngle(ServoMotor* motor) {
    return motor->angle;
}


