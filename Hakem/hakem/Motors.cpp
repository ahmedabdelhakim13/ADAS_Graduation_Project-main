#include "Motors.h"

// unsigned short Front_Right_Speed;
// unsigned short Front_Left_Speed;
short Back_Right_Speed;
short Back_Left_Speed;
short Crank_Speed;
short Idle_Speed;

void motors_init(short crankSpeed,short idleSpeed)
{
  // Initialize Motors Pins.
  pinMode(m_enable_A_pin, OUTPUT);
  pinMode(motorA_1_pin, OUTPUT);
  pinMode(motorA_2_pin, OUTPUT);
  pinMode(m_enable_B_pin, OUTPUT);
  pinMode(motorB_1_pin, OUTPUT);
  pinMode(motorB_2_pin, OUTPUT);
  // pinMode(m_enable_C_pin, OUTPUT);
  // pinMode(motorC_1_pin, OUTPUT);
  // pinMode(motorC_2_pin, OUTPUT);
  // pinMode(m_enable_D_pin, OUTPUT);
  // pinMode(motorD_1_pin, OUTPUT);
  // pinMode(motorD_2_pin, OUTPUT);

  // Initialize Crank_Speed and Idle_Speed
  Crank_Speed = crankSpeed;
  Idle_Speed = idleSpeed;

  off();
}

// unsigned short Get_FR_Speed()
// {
//   return Front_Right_Speed;
// }

// unsigned short Get_FL_Speed()
// {
//   return Front_Left_Speed;
// }

unsigned short Get_BR_Speed()
{
  return Back_Right_Speed;
}

unsigned short Get_BL_Speed()
{
  return Back_Left_Speed;
}

bool Is_Stopped()
{
  bool flag = false;
  if((Back_Right_Speed == 0) && (Back_Left_Speed == 0))
  {
    flag = true;
  }
  return flag;
}

// bool Is_Rotating_Left()
// {
//   bool flag = false;
//   if ((Front_Left_Speed < 0) && (Front_Right_Speed> 0))
//   {
//     flag = true;
//   }
//   return flag;
// }

// bool Is_Rotating_Right()
// {
//   bool flag = false;
//   if ((Front_Left_Speed > 0) && (Front_Right_Speed < 0))
//   {
//     flag = true;
//   }
//   return flag;
// }

void move(short BR_Speed,short BL_Speed)
{
  if((Back_Right_Speed == BR_Speed) && (Back_Left_Speed == BL_Speed))
  {
    // Do nothing as there is no speed change.
  }
  else
  {
    // Limit values
    // FR_Speed = constrain(FR_Speed, -255, 255);
    // FL_Speed = constrain(FL_Speed, -255, 255);
    BR_Speed = constrain(BR_Speed, -255, 255);
    BL_Speed = constrain(BL_Speed, -255, 255);

    // Prevent buzzing at low speeds
    // unsigned short MIN_FR_Speed = (Front_Right_Speed == 0) ? Crank_Speed : Idle_Speed;
    // unsigned short MIN_FL_Speed = (Front_Left_Speed  == 0) ? Crank_Speed : Idle_Speed;
    unsigned short MIN_BR_Speed = (Back_Right_Speed  == 0) ? Crank_Speed : Idle_Speed;
    unsigned short MIN_BL_Speed = (Back_Left_Speed   == 0) ? Crank_Speed : Idle_Speed;

    // Update Global Speeds
    // Front_Right_Speed = (abs(FR_Speed) < MIN_FR_Speed) ? 0 : FR_Speed;
    // Front_Left_Speed  = (abs(FL_Speed) < MIN_FL_Speed) ? 0 : FL_Speed;
    Back_Right_Speed  = (abs(BR_Speed) < MIN_BR_Speed) ? 0 : BR_Speed;
    Back_Left_Speed   = (abs(BL_Speed) < MIN_BL_Speed) ? 0 : BL_Speed;

    // if (Front_Right_Speed < 0)
    // {
    //   digitalWrite(motorA_1_pin, HIGH);
    //   digitalWrite(motorA_2_pin, LOW);
    //   analogWrite(m_enable_A_pin ,abs(Front_Right_Speed));
    // }
    // else if (Front_Right_Speed == 0)
    //   analogWrite(m_enable_A_pin, 0);
    // else
    // {
    //   digitalWrite(motorA_1_pin, LOW);
    //   digitalWrite(motorA_2_pin, HIGH);
    //   analogWrite(m_enable_A_pin, Front_Right_Speed);
    // }
    // //////////////////////////////////////////////////
    // if (Front_Left_Speed < 0)
    // {
    //   digitalWrite(motorB_1_pin, HIGH);
    //   digitalWrite(motorB_2_pin, LOW);
    //   analogWrite(m_enable_B_pin, abs(Front_Left_Speed));
    // }
    // else if (Front_Left_Speed == 0)
    //   analogWrite(m_enable_B_pin, 0);
    // else
    // {
    //   digitalWrite(motorB_1_pin, LOW);
    //   digitalWrite(motorB_2_pin, HIGH);
    //   analogWrite(m_enable_B_pin, Front_Left_Speed);
    // }
    ///////////////////////////////////////////////////
    if (Back_Right_Speed < 0)
    {
      digitalWrite(motorA_1_pin, HIGH);
      digitalWrite(motorA_2_pin, LOW);
      analogWrite(m_enable_A_pin ,abs(Back_Right_Speed));
    }
    else if (Back_Right_Speed == 0)
      analogWrite(m_enable_A_pin, 0);
    else
    {
      digitalWrite(motorA_1_pin, LOW);
      digitalWrite(motorA_2_pin, HIGH);
      analogWrite(m_enable_A_pin, Back_Right_Speed);
    }
    ////////////////////////////////////////////////////
    if (Back_Left_Speed < 0)
    {
      digitalWrite(motorB_1_pin, HIGH);
      digitalWrite(motorB_2_pin, LOW);
      analogWrite(m_enable_B_pin ,abs(Back_Left_Speed));
    }
    else if (Back_Left_Speed == 0)
      analogWrite(m_enable_B_pin, 0);
    else
    {
      digitalWrite(motorB_1_pin, LOW);
      digitalWrite(motorB_2_pin, HIGH);
      analogWrite(m_enable_B_pin, Back_Left_Speed);
    }
  }
}

void stop()
{
  digitalWrite(motorA_1_pin, LOW);
  digitalWrite(motorA_2_pin, LOW);
  digitalWrite(motorB_1_pin, LOW);
  digitalWrite(motorB_2_pin, LOW);
  digitalWrite(m_enable_A_pin, LOW);
  digitalWrite(m_enable_B_pin, LOW);
  // digitalWrite(m_enable_C_pin, LOW);
  // digitalWrite(m_enable_D_pin, LOW);
  // Front_Right_Speed = 0;
  // Front_Left_Speed = 0;
  Back_Right_Speed = 0;
  Back_Left_Speed = 0;
}

void move_forward(short Speed)
{
  move(Speed, Speed);
}

void move_backward(short Speed)
{
  move(-Speed, -Speed);
}

void off()
{
  stop();
  digitalWrite(motorA_1_pin, LOW);
  digitalWrite(motorA_2_pin, LOW);
  digitalWrite(motorB_1_pin, LOW);
  digitalWrite(motorB_2_pin, LOW);
  // digitalWrite(motorC_1_pin, LOW);
  // digitalWrite(motorC_2_pin, LOW);
  // digitalWrite(motorD_1_pin, LOW);
  // digitalWrite(motorD_2_pin, LOW);
}


// The Remaining Part was a commented code ,so I didn't convert it to C code.
// I just copied it in case we needed it.

/*

  @brief Rotate robot left.
  @param speed Robot speed (0..255).
 
void Motors::left(unsigned char speed)
{
    move(-speed, speed);
}

  @brief Rotate robot right.
  @param speed Robot speed (0..255).
 
void Motors::right(unsigned char speed)
{
    move(speed, -speed);
}

  @brief Move forward turning left.
  @param speed Robot speed (0..255).
 
void Motors::forwardLeft(unsigned char speed)
{
    move(speed/2, speed);
}

  @brief Move forward turning right.
  @param speed Robot speed (0..255).
 
void Motors::forwardRight(unsigned char speed)
{
    move(speed, speed/2);
}

  @brief Move backward turning left.
  @param speed Robot speed (0..255).
 
void Motors::backwardLeft(unsigned char speed)
{
    move(-speed/2, -speed);
}

  @brief Move backward turning right.
  @param speed Robot speed (0..255).
 
void Motors::backwardRight(unsigned char speed)
{
    move(-speed, -speed/2);
}
*/










