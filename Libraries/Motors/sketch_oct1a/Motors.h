#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#define motorA_1_pin  10
#define motorA_2_pin  11
#define motorB_1_pin    12 
#define motorB_2_pin    13
#define motorC_1_pin    14
#define motorC_2_pin    15
#define motorD_1_pin    16  
#define motorD_2_pin    17
#define m_enable_A_pin    18
#define m_enable_B_pin    19
#define m_enable_C_pin    20
#define m_enable_D_pin    21
    
class Motors
{
private:
  unsigned char m_enableA, motorA_1, motorA_2; // Front Right motors pins
  unsigned char m_enable_B, motorB_1, motorB_2; // Front Left motors pins
  unsigned char m_enable_C, motorC_1, motorC_2; // Back Right motors pins
  unsigned char m_enable_D, motorD_1, motorD_2; // Back left motors pins
  unsigned char m_crankSpeed, m_idleSpeed;     // Minimum speeds
  short m_left_Front_Speed, m_right_Front_Speed;//front motors
  short m_left_back_Speed, m_right_back_Speed; //back motors
public:
    Motors(unsigned char enableA,unsigned char motor_A_1,unsigned char motor_A_2,unsigned char enableB,unsigned char motor_B_1,unsigned char motor_B_2,unsigned char enableC,unsigned char motor_C_1,unsigned char motor_C_2,unsigned char enableD,unsigned char motor_D_1,unsigned char motor_D_2 ,unsigned char crankSpeed,unsigned char idleSpeed);
    ~Motors();
    short getLeftBackSpeed() const;
    short getRightBackSpeed() const;
    short getLeftFrontSpeed() const;
    short getRightFrontSpeed() const;

    bool isStopped() const;

    bool isRotatingLeft() const;
    bool isRotatingRight() const;

    void move(short left_FrontSpeed, short right_FrontSpeed,short right_BackSpeed,short left_BackSpeed);
    void forward(unsigned char speed);
    void backward(unsigned char speed);
    void left(unsigned char speed);
    void right(unsigned char speed);
    void forwardLeft(unsigned char speed);
    void forwardRight(unsigned char speed);
    void backwardLeft(unsigned char speed);
    void backwardRight(unsigned char speed);
    
    void off();
    void stop();
};
#endif