#include "FCW.h"

unsigned long interval = 500;
unsigned long start = 0;
unsigned long distance1 = 0;
unsigned long distance2 = 0;
short maxSpeed = 250;
short avgSpeed = 200;
short minSpeed = 150;
short currentSpeed = 0;

void FCW_init()
{
  ultrasonic_init(FU_TRIGGERPIN, FU_ECHOPIN);
  motors_init(50, 80);
}

void FCW_run()
{
  distance1 = ultrasonic_Get_Distance(FU_TRIGGERPIN, FU_ECHOPIN);
  Serial.println("distance: " + String(distance1) + " cm");
  start = millis();
  if((distance1 > 300))
  {
    Serial.println("No Car in Horizon");
    move_forward(maxSpeed);
    currentSpeed = maxSpeed;
  }
  else if((distance1 > 90) && (distance1 <= 300))
  {
    Serial.println("Distance is Within the Adequate Range");
    move_forward(avgSpeed);
    currentSpeed = avgSpeed;
  }
  else if((distance1 > 30) && (distance1 <= 90))
  {
    Serial.println("Distance Dropped Below Critical Limits, Slowing Down");
    move_forward(minSpeed);
    currentSpeed = minSpeed;
  }
  else if(distance1 <= 30)
  {
    Serial.println("Forward Collision, Stopping the Car");
    stop();
    currentSpeed = 0;
  }
}


 















