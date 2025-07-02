#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include <Arduino.h>

class UltrasonicSensor
{
  private:
    unsigned char m_triggerPin;
    unsigned char m_echoPin;
    
  public:
    UltrasonicSensor(unsigned char triggerPin, unsigned char echoPin);
    long getDistance();  // Method to get distance in cm
};

#endif
