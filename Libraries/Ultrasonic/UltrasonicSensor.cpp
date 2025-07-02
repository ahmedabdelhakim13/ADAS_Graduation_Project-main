#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(unsigned char triggerPin, unsigned char echoPin)
{
  m_triggerPin = triggerPin;
  m_echoPin = echoPin;
  
  pinMode(m_triggerPin, OUTPUT);  // Set trigger pin as output
  pinMode(m_echoPin, INPUT);      // Set echo pin as input
}

long UltrasonicSensor::getDistance()
{
  digitalWrite(m_triggerPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(m_triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(m_triggerPin, LOW);
  
  long duration = pulseIn(m_echoPin, HIGH);  // Measure pulse duration
  long distance = (duration * 0.034) / 2;    // Calculate distance in cm
  
  return distance;
}
