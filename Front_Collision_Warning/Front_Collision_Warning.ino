#include "Motors.h"
#include "UltrasonicSensor.h"

#define U_TRIGGERPIN  0
#define U_ECHOPIN     1

UltrasonicSensor frontUltrasonic(U_TRIGGERPIN, U_ECHOPIN);

Motors motorControl(m_enable_A_pin, motorA_1_pin, motorA_2_pin, 
                    m_enable_B_pin, motorB_1_pin, motorB_2_pin, 
                    m_enable_C_pin, motorC_1_pin, motorC_2_pin, 
                    m_enable_D_pin, motorD_1_pin, motorD_2_pin, 
                    50, 100);

unsigned long interval = 500;
unsigned long start = 0;
long distance1 = 0;
long distance2 = 0;
short maxSpeed = 150;
short avgSpeed = 100;
short minSpeed = 50;
short currentSpeed = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    distance1 = frontUltrasonic.getDistance();
    start = millis();

    if ((distance1 > 300) && (distance1 < 450)) {
        Serial.println("No Car in Horizon");
        motorControl.move(maxSpeed, maxSpeed);
        currentSpeed = maxSpeed;
    } 
    else if ((distance1 > 75) && (distance1 <= 300)) {
        Serial.println("Distance is Within the Adequate Range");
        motorControl.move(avgSpeed, avgSpeed);
        currentSpeed = avgSpeed;
    } 
    else if ((distance1 > 10) && (distance1 <= 75)) {
        Serial.println("Distance Dropped Below Critical Limits, Slowing Down");
        motorControl.move(minSpeed, minSpeed);
        currentSpeed = minSpeed;
    } 
    else if (distance1 <= 10) {
        Serial.println("Forward Collision, Stopping the Car");
        motorControl.stop();
    }

    while ((millis() - start) < interval);

    distance2 = frontUltrasonic.getDistance();
    
    if ((distance1 - distance2) >= 0) {
        Serial.println("Car is on the Adequate Speed");
    } 
    else if ((distance1 - distance2) < 0) {
        Serial.println("Danger, a Collision is Possible");
        currentSpeed = currentSpeed - 50;
        
        if (currentSpeed <= 0) {
            motorControl.stop();
        } else {
            motorControl.move(currentSpeed, currentSpeed);
        }
    }
}
