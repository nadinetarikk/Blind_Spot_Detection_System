#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "pico/stdlib.h"
#include "Led/LED.c"
#include "ultraSonic/ultrasonic_sensor.c"
#include "motor/motor_control.c"
#include "LDR/LDR.c"
#include "Buzzer/buzzer.c"
#include "Infrared/infrared_sensor.c"
#include "Switch/switch.c"
#define LED_PIN1 20
#define LED_PIN2 21
// void motor_control_task(void *pvParameters);
// void ultrasonic_sensor_task(void *pvParameters);
void ldr_sensor_task(void *pvParameters);
void ultrasonic_task1(void *params);
void ultrasonic_task2(void *params);
void motor_control_task(void *pvParameters);