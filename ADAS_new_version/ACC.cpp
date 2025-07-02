#include "ACC.h"

void ACC_init()
{
  FCW_init();
}

void ACC_run()
{
  int distance = ultrasonic_Get_Distance(FU_TRIGGERPIN, FU_ECHOPIN);

    // Check if the distance is less than the safe threshold
  if (distance < SAFE_DISTANCE) 
  {
    currentSpeed -= DECELERATION_RATE;
    if (currentSpeed < 0) 
    {
      currentSpeed = 0;
    }
  } 
  else 
  {
    // If safe, adjust speed towards the target speed
    if (currentSpeed < avgSpeed) 
    {
      currentSpeed += ACCELERATION_RATE;
      if (currentSpeed > avgSpeed) 
      {
        currentSpeed = avgSpeed;
      }
    } 
    else if (currentSpeed > avgSpeed) 
    {
      currentSpeed -= DECELERATION_RATE;
      if (currentSpeed < avgSpeed) 
      {
        currentSpeed = avgSpeed;
      }
    }
  }

    // Set the motor speed
    move_forward(currentSpeed);
}