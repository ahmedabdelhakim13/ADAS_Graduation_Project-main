#include "FCW.h"
#include "BSW.h"
#include "ACC.h"
#include "VSU.h"
#include "APA.h"

void setup() {
  ACC_init();
  BSW_init();
  VSU_init();
  APA_init();
  Serial.begin(115200);
}

void loop() {
  // APA_run();
  // Always check for messages from ESP32
  VSU_run();
  if (!isInSteeringMode()) {
    // Only run features if not in steering mode
    FCW_run();
    BSW_run();
    ACC_run();
    // APA_run();
    if(flag)
    {
      stop();
      delay(5000);
      move_forward(200);
    }
    delay(20);
  }
}

/*int main(){
  FCW_init();
  BSW_init();
  Serial.begin(9600);
  while(1)
  {
    FCW_run();
    BSW_run();
    delay(1000);
  }
  return 0;
}*/
