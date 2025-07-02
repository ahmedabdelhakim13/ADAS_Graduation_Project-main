#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

#define motorA_1_pin   PA2
#define motorA_2_pin   PA3
#define motorB_1_pin   PA4
#define motorB_2_pin   PA5
// #define motorC_1_pin   14
// #define motorC_2_pin   15
// #define motorD_1_pin   16  
// #define motorD_2_pin   17
#define m_enable_A_pin PA6
#define m_enable_B_pin PA7
// #define m_enable_C_pin 20
// #define m_enable_D_pin 21

void motors_init(short crankSpeed,short idleSpeed);
// unsigned short Get_FR_Speed();
// unsigned short Get_FL_Speed();
unsigned short Get_BR_Speed();
unsigned short Get_BL_Speed();
bool Is_Stopped();
bool Is_Rotating_Left();
bool Is_Rotating_Right();
void move(short BR_Speed,short BL_Speed);
void stop();
void move_forward(short Speed);
void move_backward(short Speed);
void off();

#endif