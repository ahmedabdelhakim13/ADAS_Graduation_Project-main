#include "APA.h"

unsigned long apa_interval = 1000; // Check every 500ms for real-time operation
unsigned long apa_start = 0;
unsigned long apa_collection_interval = 30000; // Collect reading every 30 seconds
unsigned long apa_collection_start = 0;

#define MAX_READINGS 10 // Size of the distance readings array
unsigned long distance_readings[MAX_READINGS] = {0}; // Array to store distance readings
int reading_index = 0; // Current index in the array
int reading_count = 0; // Number of valid readings in the array

void APA_init()
{
  ultrasonic_init(REAR_TRIGGER_PIN, REAR_ECHO_PIN);
  motors_init(100, 50); // Initialize motors with crank speed 100 and idle speed 50
}

void APA_run()
{
  // Real-time APA checks every 500ms
    unsigned long distance = ultrasonic_Get_Distance(REAR_TRIGGER_PIN, REAR_ECHO_PIN);
    Serial.print("Rear Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 110) {
      Serial.println("Safe Distance: No obstacles detected behind");
      move_backward(150); // Move backward at speed 150 (adjustable)
      Serial.println("Action: Moving backward");
      }
    else if (distance <= 40) {
      Serial.println("Critical: Too close to obstacle behind, stopping!");
      stop(); // Ensure the car is stopped
      Serial.println("Action: Stopping car");
    }

    apa_start = millis();
  

  // Collect distance reading every 30 seconds
  if (millis() - apa_collection_start >= apa_collection_interval) {
    unsigned long distance = ultrasonic_Get_Distance(REAR_TRIGGER_PIN, REAR_ECHO_PIN);
    
    // Store the reading in the array
    distance_readings[reading_index] = distance;
    reading_index = (reading_index + 1) % MAX_READINGS; // Circular buffer
    if (reading_count < MAX_READINGS) {
      reading_count++; // Increment until array is full
    }

    // Analyze readings for danger condition
    int danger_count = 0;
    for (int i = 0; i < reading_count; i++) {
      if (distance_readings[i] <= 90) {
        danger_count++; // Count readings in Warning or Critical range
      }
    }

   // Check if danger count exceeds threshold (half of array size)
    int threshold = MAX_READINGS / 2; // e.g., 5 for 10 readings
    if (reading_count >= MAX_READINGS && danger_count > threshold) {
      Serial.println("Very Dangerous, Too Close!");
      stop(); // Stop the car if consistently too close
      Serial.println("Action: Stopping car due to consistent proximity");
    }

   // Debug: Print current readings array (optional, for testing)
    Serial.print("Distance Readings: ");
    for (int i = 0; i < reading_count; i++) {
      Serial.print(distance_readings[i]);
      Serial.print(" cm");
      if (i < reading_count - 1) Serial.print(", ");
    }
    Serial.println();

    apa_collection_start = millis();
  }
}

