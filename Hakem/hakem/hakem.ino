#include "Motors.h"
#include "Ultrasonic.h"

// Define the ultrasonic sensor pins
const unsigned char triggerPin = 12;
const unsigned char echoPin = 13;

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  
  // Initialize motors and ultrasonic
  motors_init(100, 50);
  ultrasonic_init(triggerPin, echoPin);
}

void loop() {
  // Get the distance
  int distance = ultrasonic_Get_Distance(triggerPin, echoPin);

  // Print the distance to Serial Monitor
  Serial.println(distance);
  delay(500);  // Delay to reduce Serial flooding
}
