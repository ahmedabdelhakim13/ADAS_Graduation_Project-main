#include "Motors.h"
/**
* @brief This following lines are assigning pins from the stm32F103C8t  
*        to the four motors of the car
*/

/**
 * @brief Construct a new Motors::Motors object   (Constructor of the class)
 * @param enableA Right motor enable.
 * @param motorA  Left motor motorA .
 * @param motorB Left motor motorB .
 * @param enableB Left motor enable.
 * @param motorC  Left motor motorC.
 * @param motorD  Left motor motorD .
 * @param enableC Left motor enable.
 * @param enableD Left motor enable.
 * @param crankSpeed Minimun crank speed.
 * @param idleSpeed Minimum idle speed.
 */

Motors::Motors(unsigned char enableA, 
 unsigned char motorA_1 ,
 unsigned char motorA_2 ,
 unsigned char enableB,
 unsigned char motorB_1 ,
 unsigned char motorB_2 , 
 unsigned char enableC ,
 unsigned char motorC_1 ,
 unsigned char motorC_2 , 
unsigned char enableD ,
 unsigned char motorD_1 ,
 unsigned char motorD_2 ,  
 unsigned char crankSpeed, 
 unsigned char idleSpeed)
    : m_enableA{enableA},
     motorA_1{motorA_1}, 
     motorA_2{motorA_2}, 
     m_enable_B{enableB},
     motorB_1{motorB_1},
     motorB_2{motorB_2},
     m_enable_C{enableC},
    motorC_1{motorC_1},
    motorC_2{motorC_2},
    m_enable_D{enableD}, 
    motorD_1{motorD_1}, 
    motorD_2{motorD_2}, 
    m_crankSpeed{crankSpeed}, 
    m_idleSpeed{idleSpeed}, 
    m_left_Front_Speed{0}, 
    m_right_Front_Speed{0},
    m_left_back_Speed{0},
    m_right_back_Speed{0}
{
    pinMode(m_enable_A_pin, OUTPUT);
    pinMode(motorA_1_pin, OUTPUT);
    pinMode(motorA_2_pin, OUTPUT);
    pinMode(m_enable_B_pin, OUTPUT);
    pinMode(motorB_1_pin, OUTPUT);
    pinMode(motorB_2_pin, OUTPUT);
    pinMode(m_enable_C_pin, OUTPUT);
    pinMode(motorC_1_pin, OUTPUT);
    pinMode(motorC_2_pin, OUTPUT);
    pinMode(m_enable_D_pin, OUTPUT);
    pinMode(motorD_1_pin, OUTPUT);
    pinMode(motorD_2_pin, OUTPUT);
    off();
}
/**
* @brief Destroy the motors object  (Destructor of the class)
* @param nothing
*
*/
Motors::~Motors()
{
    off();
}

/**
 * @brief Get left front motors speed.
 * @return short left motors speed.
 */

 short Motors::getLeftBackSpeed() const
{
    return  m_left_back_Speed;
}

/**
 * @brief Get left frontmotors speed.
 * @return short left motors speed.
 */
  short Motors::getLeftFrontSpeed() const
  {
    return m_left_Front_Speed;
  }
/**
 * @brief Get right back motors speed.
 * @return short right motors speed.
 */

short Motors::getRightBackSpeed() const
{
    return m_right_back_Speed;
}

/**
 * @brief Get right back motors speed.
 * @return short right motors speed.
 */

short Motors::getRightFrontSpeed() const
{
    return m_right_Front_Speed;
}

/**
 * @brief Return if motors are stopped.
 * @return true Motors stopped.
 * @return false Motors speeds non zero.
 */

bool Motors::isStopped() const
{
    if ((m_left_Front_Speed == 0) && (m_right_Front_Speed == 0) &&(m_left_back_Speed == 0)&&(m_right_back_Speed == 0))
        return true;
    return false;
}

/**
 * @brief Return if robot is rotating left.
 * @return true Robot rotating left.
 * @return false Robot not rotating left.
 */

bool Motors::isRotatingLeft() const
{
    if ((m_left_Front_Speed < 0) && (m_right_Front_Speed> 0))
        return true;
    return false;
}

/**
 * @brief Return if robot is rotating right.
 * @return true Robot rotating right.
 * @return false Robot not rotating right.
 */

bool Motors::isRotatingRight() const
{
    if ((m_left_Front_Speed > 0) && (m_right_Front_Speed < 0))
        return true;
    return false;
}

/**
 * @brief Drive the motors with a PWM signal.
 * @param left front Speed PWM value: -255..255.
 * @param right front Speed PWM value: -255..255.
 * @param right back Speed PWM value: -255..255.
 * @param right back Speed PWM value: -255..255.
 */

void Motors::move(short left_FrontSpeed, short right_FrontSpeed,short right_BackSpeed,short left_BackSpeed)
{
    // Return if no speed change
    if ((m_left_Front_Speed  == left_FrontSpeed) && (m_right_Front_Speed == right_FrontSpeed)&& (m_left_back_Speed == left_BackSpeed)&& (m_right_back_Speed == right_BackSpeed))
        return;

    // Limit values
    left_FrontSpeed = constrain(left_FrontSpeed, -255, 255);
    right_FrontSpeed = constrain(right_FrontSpeed, -255, 255);
    right_BackSpeed = constrain(right_BackSpeed, -255, 255);
    left_BackSpeed  = constrain(left_BackSpeed, -255, 255);

    // Prevent buzzing at low speeds
    unsigned char minLeft_FrontSpeed = (m_left_Front_Speed == 0) ? m_crankSpeed : m_idleSpeed;
    unsigned char minRight_FrontSpeed = (m_right_Front_Speed == 0) ? m_crankSpeed : m_idleSpeed;
    unsigned char minLeft_BackSpeed = (m_left_back_Speed == 0) ? m_crankSpeed : m_idleSpeed;
    unsigned char minRight_BackSpeed = (m_right_back_Speed == 0) ? m_crankSpeed : m_idleSpeed;

    // Member variables update first
    m_left_Front_Speed = (abs(left_FrontSpeed) < minLeft_FrontSpeed) ? 0 : left_FrontSpeed;
    m_right_Front_Speed = (abs( right_FrontSpeed) < minRight_FrontSpeed) ? 0 : right_FrontSpeed;
    m_left_back_Speed = (abs( right_FrontSpeed) < minRight_FrontSpeed) ? 0 : right_FrontSpeed;
    m_right_Front_Speed = (abs( right_FrontSpeed) < minRight_FrontSpeed) ? 0 : right_FrontSpeed;

    if (m_left_Front_Speed < 0)
    {
        digitalWrite(motorB_1_pin, HIGH);
        digitalWrite(motorB_2_pin, LOW);
        analogWrite(m_enable_B_pin, abs(m_left_Front_Speed));
    }
    else if (m_left_Front_Speed == 0)
        analogWrite(m_enable_B_pin, 0);
    else
    {
        digitalWrite(motorB_1_pin, LOW);
        digitalWrite(motorB_2_pin, HIGH);
        analogWrite(m_enable_B_pin, m_left_Front_Speed);
    }
       ///////////////////////////////
    if (m_right_Front_Speed < 0)
    {
        digitalWrite(motorA_1_pin, HIGH);
        digitalWrite(motorA_2_pin, LOW);
        analogWrite(m_enable_A_pin ,abs(m_right_Front_Speed));
    }
    else if (m_right_Front_Speed == 0)
        analogWrite(m_enable_A_pin, 0);
    else
    {
        digitalWrite(motorA_1_pin, LOW);
        digitalWrite(motorA_2_pin, HIGH);
        analogWrite(m_enable_A_pin, m_right_Front_Speed);
    }
    ////////////////////////////////////////
    if (m_left_back_Speed < 0)
    {
        digitalWrite(motorC_1_pin, HIGH);
        digitalWrite(motorC_2_pin, LOW);
        analogWrite(m_enable_C_pin ,abs(m_right_Front_Speed));
    }
    else if (m_left_back_Speed== 0)
        analogWrite(m_enable_C_pin, 0);
    else
    {
        digitalWrite(motorC_1_pin, LOW);
        digitalWrite(motorC_2_pin, HIGH);
        analogWrite(m_enable_C_pin, m_left_back_Speed);
    }
    /////////////////////////////////////////
         if (m_right_back_Speed < 0)
    {
        digitalWrite(motorD_1_pin, HIGH);
        digitalWrite(motorD_2_pin, LOW);
        analogWrite(m_enable_D_pin ,abs(m_right_back_Speed));
    }
    else if (m_right_back_Speed== 0)
        analogWrite(m_enable_D_pin, 0);
    else
    {
        digitalWrite(motorD_1_pin, LOW);
        digitalWrite(motorD_2_pin, HIGH);
        analogWrite(m_enable_D_pin, m_right_back_Speed);
    }
}

/**
 * @brief Stop motors.
 */
void Motors::stop()
{
    digitalWrite(m_enable_A_pin, LOW);
    digitalWrite(m_enable_B_pin, LOW);
    digitalWrite(m_enable_C_pin, LOW);
    digitalWrite(m_enable_D_pin, LOW);
    m_left_Front_Speed = 0;
    m_right_Front_Speed = 0;
    m_left_back_Speed=0;
    m_right_back_Speed=0;
}
/**
 * @brief Move robot forward.
 * @param speed Robot speed (0..255).
 */ 
 void Motors::forward(unsigned char speed)
{
    move(speed, speed,speed,speed);
}

/**
 * @brief Move robot backward.
 * @param speed Robot speed (0..255).
 */ 
void Motors::backward(unsigned char speed)
{
    move(-speed, -speed,-speed,-speed);
}


/**
 * @brief Stop and turn motors off.
*/ 
void Motors::off()
{
    stop();
    digitalWrite(motorA_1_pin, LOW);
    digitalWrite(motorA_2_pin, LOW);
    digitalWrite(motorB_1_pin, LOW);
    digitalWrite(motorB_2_pin, LOW);
    digitalWrite(motorC_1_pin, LOW);
    digitalWrite(motorC_2_pin, LOW);
    digitalWrite(motorD_1_pin, LOW);
    digitalWrite(motorD_2_pin, LOW);
}

/*

/**
 * @brief Rotate robot left.
 * @param speed Robot speed (0..255).
 
void Motors::left(unsigned char speed)
{
    move(-speed, speed);
}

/**
 * @brief Rotate robot right.
 * @param speed Robot speed (0..255).
 
void Motors::right(unsigned char speed)
{
    move(speed, -speed);
}

/**
 * @brief Move forward turning left.
 * @param speed Robot speed (0..255).
 
void Motors::forwardLeft(unsigned char speed)
{
    move(speed/2, speed);
}

/**
 * @brief Move forward turning right.
 * @param speed Robot speed (0..255).
 
void Motors::forwardRight(unsigned char speed)
{
    move(speed, speed/2);
}

/**
 * @brief Move backward turning left.
 * @param speed Robot speed (0..255).
 
void Motors::backwardLeft(unsigned char speed)
{
    move(-speed/2, -speed);
}

/**
 * @brief Move backward turning right.
 * @param speed Robot speed (0..255).
 
void Motors::backwardRight(unsigned char speed)
{
    move(-speed, -speed/2);
}
*/


