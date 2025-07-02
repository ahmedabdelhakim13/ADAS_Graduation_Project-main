#include "BSW.h"

unsigned char R_flag = false;
unsigned char L_flag = false;
unsigned long R_distance = 0;
unsigned long L_distance = 0;

const int num_of_Readings = 10;
unsigned long R_readings[num_of_Readings];
unsigned long L_readings[num_of_Readings]; 
long R_index = 0;
long L_index = 0;                        
unsigned long R_total = 0;
unsigned long L_total = 0;              
unsigned long R_average = 0;
unsigned long L_average = 0;

unsigned char R_oldestReadingPosition = 0;
unsigned char L_oldestReadingPosition = 0;
unsigned char R_newReadingPosition = 0;
unsigned char L_newReadingPosition = 0;

unsigned char divisor = 0;

void BSW_init()
{
  ultrasonic_init(RU_TRIGGERPIN, RU_ECHOPIN);
  ultrasonic_init(LU_TRIGGERPIN, LU_ECHOPIN);
  // pinMode(Buzzer_PIN, OUTPUT);
  // digitalWrite(Buzzer_PIN, LOW);

  for (int i = 0; i < num_of_Readings; i++) 
  {
    R_readings[i] = 0;
    L_readings[i] = 0;
  }
}

void BSW_run()
{
  // Calculating the distance in the right side.
  R_distance = ultrasonic_Get_Distance(RU_TRIGGERPIN, RU_ECHOPIN);

  if( R_index >= num_of_Readings)
  {
    R_oldestReadingPosition = R_index % num_of_Readings;
    R_total = R_total - R_readings[R_oldestReadingPosition];
  }

  // for the the first 9 readings the distance value will be wrong
  R_newReadingPosition = R_index % num_of_Readings;
  R_readings[R_newReadingPosition] = R_distance;
  R_total = R_total + R_distance;
  divisor = (R_index < num_of_Readings) ? (R_index + 1) : num_of_Readings;
  R_average = R_total / divisor;
  R_index++;


  if(R_average <= 20)
  {
   R_flag = true;
  }
  else
  {
    R_flag = false;
  }

  // Calculating the distance in the left side.
  L_distance = ultrasonic_Get_Distance(LU_TRIGGERPIN, LU_ECHOPIN);


  if( L_index >= num_of_Readings)
  {
    L_oldestReadingPosition = L_index % num_of_Readings;
    L_total = L_total - L_readings[L_oldestReadingPosition];
  }

  // for the the first 9 readings the distance value will be wrong
  L_newReadingPosition = L_index % num_of_Readings;
  L_readings[L_newReadingPosition] = L_distance;
  L_total = L_total + L_distance;
  divisor = (L_index < num_of_Readings) ? (L_index + 1) : num_of_Readings;
  L_average = L_total / divisor;
  L_index++;


  if(L_average <= 20)
  {
   L_flag = true;
  }
  else
  {
    L_flag = false;
  }


  // Output a message to the user.

  Serial.println("Right sensor average distance: " + String(R_average) + " cm");

  Serial.println("Left sensor average distance: " + String(L_average) + " cm");


  if( (R_flag == true) && (L_flag == true) )
  {
    Serial.println("There are cars on both the right and left sides.");
    // digitalWrite(Buzzer_PIN, HIGH);
    // delay(1000);
    // digitalWrite(Buzzer_PIN, LOW);
  }
  else if( (R_flag == true) && (L_flag == false) )
  {
    Serial.println("There are a car on the right side.");
    // digitalWrite(Buzzer_PIN, HIGH);
    // delay(1000);
    // digitalWrite(Buzzer_PIN, LOW);
  }
  else if( (R_flag == false) && (L_flag == true) )
  {
    Serial.println("There are a car on the left side.");
    // digitalWrite(Buzzer_PIN, HIGH);
    // delay(1000);
    // digitalWrite(Buzzer_PIN, LOW);
  }
  else
  {
    // digitalWrite(Buzzer_PIN, LOW);
    Serial.println("There are no cars on either side.");
  }
}


