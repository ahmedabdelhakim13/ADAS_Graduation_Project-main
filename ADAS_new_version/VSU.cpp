#include <Servo.h>
#include "VSU.h"

HardwareSerial Serial3(PB11, PB10);  // RX, TX

bool flag = false;

enum VSU_Mode {
  NORMAL_MODE,
  STEERING_MODE
};

char incomingBuffer[64];
int bufIndex = 0;

static VSU_Mode currentMode = NORMAL_MODE;
steeringWheel SW1;
Servo steeringServo;
Message message;
SensorData receivedData;

// Function to handle USART3 reception and return the received message
static bool handleUSART3Reception() {
  while (Serial3.available()) {
    char c = Serial3.read();
    if (c == '\n') {
      if (bufIndex > 0 && incomingBuffer[bufIndex - 1] == '\r') {
        bufIndex--;   // Remove '\r'
      }
      incomingBuffer[bufIndex] = '\0';  // Null-terminate
      bufIndex = 0;                     // Reset index for next message
      return true;
    }
    if (bufIndex < sizeof(incomingBuffer) - 1) {
      incomingBuffer[bufIndex++] = c;
    }
  }
  return false;
}

// Function to get steering and pedal data
void receiveSteeringAndPedal() {
  char *commaPos = strchr(incomingBuffer, ',');
  if (commaPos != nullptr) {
    *commaPos = '\0';   // Split the string at comma
    SW1.steeringValue = atof(incomingBuffer);
    SW1.pedalValue = atof(commaPos + 1);
  }
}

// Control the car prototype according to the steering and pedal values 
static void controlVehicle(float steering, float pedal) {
  // Update the servo angle -> assume steering value can be [-1, 1]
  steering = - steering;
  int angle = map(steering * 100, -100, 100, 40, 140);
  steeringServo.write(angle);
  // Update the motors speeds
  int speed = abs(pedal) * 255;
  if (pedal > 0.3) {
    move_forward(speed);
  } else if (pedal < -0.3) {
    move_backward(speed);
  } else {
    stop();
  }
}

void VSU_init() {
  // UART3: Communication with ESP32
  Serial3.begin(9600);    // USART3: PB11 (RX), PB10 (TX)
  Serial3.setTimeout(100);

  steeringServo.attach(ServoPin);
}

void VSU_run() {
  if (handleUSART3Reception()) {
    // these 2 lines for testing
    Serial.print("Raw incoming buffer: ");
    Serial.println(incomingBuffer);
    if (strcmp(incomingBuffer, "STEERING_MODE") == 0) {
      currentMode = STEERING_MODE;
      Serial.println("STM32 Entered STEERING MODE");
    } 
    else if (strcmp(incomingBuffer, "NORMAL_MODE") == 0) {
      currentMode = NORMAL_MODE;
      Serial.println("STM32 Exiting STEERING MODE");
    }
    else if(strcmp(incomingBuffer, "Pedestrian wants to pass") == 0) {
      flag = true;
    }
    else if(strcmp(incomingBuffer, "Drowsy") == 0) {
      Serial.println("Drowsness detected.");
      Serial.println("Stopping the car.");
      stop();
      delay(3000);
      move_forward(150);    // test the drowsness feature -> 22/6/2025
    }
    else if(strcmp(incomingBuffer, "Stop") == 0) {
      Serial.println("Stop sign is detected!");
      Serial.println("Stopping the car.");
      stop();
      delay(3000);
    }
    else if (strcmp(incomingBuffer, "Emergency") == 0) {
      Serial.println("Emergency Vehicle Detected");
      steeringServo.write(140);
      delay(1000);
      steeringServo.write(40);
      delay(1000);
      steeringServo.write(90);
    }
    else {
      if (currentMode == STEERING_MODE) {
        receiveSteeringAndPedal();
        // Debug
        Serial.print("Steering: "); Serial.print(SW1.steeringValue);
        Serial.print(" | Pedal: "); Serial.println(SW1.pedalValue);
        // Control motor here
        controlVehicle(SW1.steeringValue, SW1.pedalValue);
      } else {
        Serial.print("STM32 Unrecognized message: ");
        Serial.println(incomingBuffer);
      }
    }
  }
}

bool isInSteeringMode() {
  return currentMode == STEERING_MODE;
}
