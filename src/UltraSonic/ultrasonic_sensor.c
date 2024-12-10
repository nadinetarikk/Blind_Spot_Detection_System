#include "ultrasonic_sensor.h"

void setup_ultrasonic() {
    // Set TRIGGER_PIN as output
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);

    // Set ECHO_PIN as input
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

        // Set TRIGGER_PIN as output
    gpio_init(TRIGGER_PIN2);
    gpio_set_dir(TRIGGER_PIN2, GPIO_OUT);

    // Set ECHO_PIN as input
    gpio_init(ECHO_PIN2);
    gpio_set_dir(ECHO_PIN2, GPIO_IN);
}

void send_trigger_pulse() {
    gpio_put(TRIGGER_PIN, 1);  // Set Trigger high
    sleep_us(10);               // Wait for 10µs
    gpio_put(TRIGGER_PIN, 0);  // Set Trigger low
    
}

void send_trigger_pulse2() {
    gpio_put(TRIGGER_PIN2, 1);  // Set Trigger high
    sleep_us(10);               // Wait for 10µs
    gpio_put(TRIGGER_PIN2, 0);  // Set Trigger low
    
}


uint32_t measure_echo_time() {
    absolute_time_t start_wait = get_absolute_time();
    
    // Wait for Echo to go high or timeout
    while (gpio_get(ECHO_PIN) == 0) {
        if (absolute_time_diff_us(start_wait, get_absolute_time()) > TIMEOUT_US) {
            return 0; // Timeout occurred
        }
    }

    absolute_time_t start_time = get_absolute_time(); // Start timing

    // Wait for Echo to go low or timeout
    while (gpio_get(ECHO_PIN) == 1) {
        if (absolute_time_diff_us(start_time, get_absolute_time()) > TIMEOUT_US) {
            return 0; // Timeout occurred
        }
    }

    absolute_time_t end_time = get_absolute_time(); // Stop timing

    // Calculate pulse width (in microseconds)
    return (uint32_t)absolute_time_diff_us(start_time, end_time);
}

uint32_t measure_echo_time2() {
    absolute_time_t start_wait = get_absolute_time();
    
    // Wait for Echo to go high or timeout
    while (gpio_get(ECHO_PIN2) == 0) {
        if (absolute_time_diff_us(start_wait, get_absolute_time()) > TIMEOUT_US) {
            return 0; // Timeout occurred
        }
    }

    absolute_time_t start_time = get_absolute_time(); // Start timing

    // Wait for Echo to go low or timeout
    while (gpio_get(ECHO_PIN2) == 1) {
        if (absolute_time_diff_us(start_time, get_absolute_time()) > TIMEOUT_US) {
            return 0; // Timeout occurred
        }
    }

    absolute_time_t end_time = get_absolute_time(); // Stop timing

    // Calculate pulse width (in microseconds)
    return (uint32_t)absolute_time_diff_us(start_time, end_time);
}


float calculate_distance(uint32_t echo_time) {
    // Speed of sound in cm/us
    const float speed_of_sound = 0.0343;
    // Distance = (time * speed) / 2 (because it’s a round trip)
    return (echo_time * speed_of_sound) / 2;
}