#include "Ultrasonic.h"

void ultrasonic_init(unsigned char triggerPin, unsigned char echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Set trigger pin as output
  pinMode(echoPin, INPUT);      // Set echo pin as input
}

unsigned long ultrasonic_Get_Distance(unsigned char triggerPin, unsigned char echoPin)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);   // Measure pulse duration
  unsigned long distance = (duration * 0.034) / 2;    // Calculate distance in cm
  
  return distance;
}