#include "SWclient.h"

void setup() {
  SWclient_init();

}

void loop() {
  while (SWFlag || SWISR_trigger)
  {
    SWclient_send_data();
  }
  // Remaining functions of ESP (Fixed Station)
}
