#include "SWserver.h"

void setup() {
  SWserver_init();

}

void loop() {
  while (SWFlag || SWISR_trigger)
  {
    SWserver_receive_data();
  }
  // Remaining functions of ESP (Main Car)
}