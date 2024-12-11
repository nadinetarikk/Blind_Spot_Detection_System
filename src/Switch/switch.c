#include "switch.h"

void switch_init(){
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);       // Set switch pin as input
}
bool get_status(){
    return gpio_get(SWITCH_PIN);
}