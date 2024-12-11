#include "pico/stdlib.h"

// Define pins for SPDT switch and LED
#define SWITCH_PIN 22  // GPIO pin for the SPDT switch (Common pin)

void switch_init();
bool get_status();