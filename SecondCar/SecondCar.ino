//#include "EmergencySensor.h"
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <esp_now.h>
//#include <WiFi.h>
#include <Wire.h>
//#include <MPU6050.h>
//#include <TinyGPS++.h>
//#include <HardwareSerial.h>


BluetoothSerial serialBT;

char BTsignal;
int Speed = 100;

// Motor control pins
#define enA 25  // Ensure this is a PWM pin
#define enB 26  // Ensure this is a PWM pin
#define INT1 13
#define INT2 12
#define INT3 14
#define INT4 27  // Changed from 20 to 19 (check for valid digital pin)

void setup() {
    Serial.begin(115200);

    if (!serialBT.begin("ADAS-Project")) {
        //Serial.println("Bluetooth initialization failed! Retrying...");
        delay(2000);
    }
    //Serial.println("Bluetooth is ready. Pair with 'ADAS-Project'.");

    pinMode(INT1, OUTPUT);
    pinMode(INT2, OUTPUT);
    pinMode(INT3, OUTPUT);
    pinMode(INT4, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);

    stop();  // Ensure motors are stopped at startup
}

void loop() {
    if (serialBT.available()) {
        BTsignal = serialBT.read();
        //Serial.println(BTsignal);

        // Speed adjustment (0-9 sets speed, 'q' sets max speed)
        if (BTsignal >= '0' && BTsignal <= '9') {
            Speed = 100 + (BTsignal - '0') * 10;
        } else if (BTsignal == 'q') {
            Speed = 225;
        }

        // Car movement controls
        switch (BTsignal) {
            case 'F': forward(); break;
            case 'B': backward(); break;
            case 'L': left(); break;
            case 'R': right(); break;
            case 'S': stop(); break;
        }
    }
    delay(10);  // Small delay to avoid excessive CPU usage
}

// Movement functions
void forward() {
    analogWrite(enA, 200);
    analogWrite(enB, 200);
    digitalWrite(INT1, HIGH);
    digitalWrite(INT2, LOW);
    digitalWrite(INT3, LOW);
    digitalWrite(INT4, HIGH);
}

void backward() {
    analogWrite(enA, Speed);
    analogWrite(enB, Speed);
    digitalWrite(INT1, LOW);
    digitalWrite(INT2, HIGH);
    digitalWrite(INT3, HIGH);
    digitalWrite(INT4, LOW);
}

void left() {
    analogWrite(enA, Speed);
    analogWrite(enB, Speed);
    digitalWrite(INT1, HIGH);
    digitalWrite(INT2, LOW);
    digitalWrite(INT3, HIGH);
    digitalWrite(INT4, LOW);
}

void right() {
    analogWrite(enA, Speed);
    analogWrite(enB, Speed);
    digitalWrite(INT1, LOW);
    digitalWrite(INT2, HIGH);
    digitalWrite(INT3, LOW);
    digitalWrite(INT4, HIGH);
}

void stop() {
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    digitalWrite(INT1, LOW);
    digitalWrite(INT2, LOW);
    digitalWrite(INT3, LOW);
    digitalWrite(INT4, LOW);
}
