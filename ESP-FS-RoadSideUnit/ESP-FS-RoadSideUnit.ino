#include "RSU.h"

void setup() {
  RSU_init();
}

void loop() {
  RSU_send_data();
}
