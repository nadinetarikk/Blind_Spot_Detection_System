// #include "main.h"

// // Function prototypes for FreeRTOS tasks
// void motor_control_task(void *pvParameters);
// void ultrasonic_sensor_task(void *pvParameters);
// void ldr_sensor_task(void *pvParameters);
// void led_buzzer_task(void *pvParameters);

// // Global variables to share data between tasks

// // float distance1 = 0, distance2 = 0;
// float light_intensity = 0;

// int main()
// {
//     // Initialize stdio and peripherals
//     stdio_init_all();
//     setup_ldr_gl5528();
//     setup_ultrasonic();
//     infrared_sensor_init();
//     motor_init();
//     buzzer_init();
//     pico_led_init(20);
//     pico_led_init(21);

//     // Create FreeRTOS tasks
//     // xTaskCreate(motor_control_task, "Motor Control", 1024, NULL, 1, NULL);
//     xTaskCreate(ultrasonic_sensor_task, "Ultrasonic Sensor", 1024, NULL, 1, NULL);
//     // xTaskCreate(ldr_sensor_task, "LDR Sensor", 1024, NULL, 1, NULL);
//     //  xTaskCreate(led_buzzer_task, "LED and Buzzer", 1024, NULL, 1, NULL);

//     // Start the FreeRTOS scheduler
//     vTaskStartScheduler();

//     // The program should never reach here
//     while (1)
//     {
//         sleep_ms(1000);
//     }
//     return 0;
// }

// // Task to control motors based on the infrared sensor

// // Task to measure distances using ultrasonic sensors
// void ultrasonic_sensor_task(void *pvParameters)
// {
//     while (1)
//     {
//         send_trigger_pulse();

//         // Measure and calculate distances for both sensors
//         float distance1 = calculate_distance(measure_echo_time());
//         // float distance2 = calculate_distance(measure_echo_time2());

//         // Control LED based on distance1 (and optionally distance2)
//         if (distance1 < 10) // Ensure valid distance
//         {
//             pico_set_led(20, true);
//         }
//         else
//         {
//             pico_set_led(20, false);
//         }

//         send_trigger_pulse2();

//         // Measure and calculate distances for both sensors
//         float distance2 = calculate_distance(measure_echo_time2());

//         // Optional: Additional logic for distance2
//         if (distance2 < 10)
//         {
//             pico_set_led(21, true); // Example for a second LED
//         }
//         else
//         {
//             pico_set_led(21, false);
//         }

//         // Debugging output
//         printf("Distance1: %.2f cm, Distance2: %.2f cm\n", distance1, distance2);

//         // Delay to run task every 200 ms
//         vTaskDelay(200 / portTICK_PERIOD_MS);
//     }
// }

// // Task to read light intensity from the LDR sensor

// // Task to control the LED and buzzer based on sensor data
// void led_buzzer_task(void *pvParameters) {
//     while (1) {
//         if (light_intensity >= 50 && (distance1 < 5 || distance2 < 5)) {
//             //buzzer_beep(5000, 2000);
//         } else if (light_intensity < 50 && (distance1 < 5 || distance2 < 5)) {
//             pico_set_led(20, true);
//         } else {
//             pico_set_led(20, false);
//         }
//         vTaskDelay(100 / portTICK_PERIOD_MS); // Run every 100ms
//     }
// }

#include "main.h"

// Global variables to store sensor readings (can be shared among tasks)
float ultrasonic1_distance = 0.0;
float ultrasonic2_distance = 0.0;
float ldr_intensity = 0;

void ultrasonic_task1(void *params)
{
    while (true)
    {
        send_trigger_pulse();
        ultrasonic1_distance = calculate_distance(measure_echo_time());

        if (ultrasonic1_distance <= 3)
        {
            if (ldr_intensity < 40)
            {
                pico_set_led(20, true); // Turn on LED 1
            }
            else
            {
                buzzer_beep(5000, 100); // Turn on Buzzer 1
            }
        }
        else
        {
            pico_set_led(20, false); // Turn off LED 1
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Wait 100ms
    }
}

void ultrasonic_task2(void *params)
{
    while (true)
    {
        send_trigger_pulse2();
        ultrasonic2_distance = calculate_distance(measure_echo_time2());

        if (ultrasonic2_distance <= 3)
        {
            if (ldr_intensity < 40)
            {
                pico_set_led(21, true); // Turn on LED 2
            }
            else
            {
                buzzer_beep2(5000, 100); // Turn on Buzzer 2
            }
        }
        else
        {
            pico_set_led(21, false); // Turn off LED 2
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Wait 100ms
    }
}

void ldr_sensor_task(void *pvParameters)
{
    while (1)
    {
        uint16_t raw_value = read_ldr_gl5528_raw();
        float voltage = ldr_gl5528_to_voltage(raw_value);
        float resistance = ldr_gl5528_to_resistance(voltage);
        ldr_intensity = ldr_gl5528_to_light_intensity(resistance);

        vTaskDelay(300 / portTICK_PERIOD_MS); // Run every 300ms
    }
}

void motor_control_task(void *pvParameters)
{
    while (1)
    {
        if (!infrared_sensor_is_triggered())
        {
            motor_stop(FRONT_LEFT);
            motor_stop(BACK_LEFT);
            motor_stop(FRONT_RIGHT);
            motor_stop(BACK_RIGHT);
        }
        else
        {
            motor_control(100, true, FRONT_LEFT);
            motor_control(100, true, BACK_LEFT);
            motor_control(100, true, FRONT_RIGHT);
            motor_control(100, true, BACK_RIGHT);
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Run every 100ms
    }
}

int main()
{
    stdio_init_all();
    setup_ldr_gl5528();
    setup_ultrasonic();
    infrared_sensor_init();
    motor_init();
    buzzer_init();
    buzzer_init2();
    pico_led_init(LED_PIN1);
    pico_led_init(LED_PIN2);

    // Create tasks
    xTaskCreate(ldr_sensor_task, "LDR Task", 1024, NULL, 1, NULL);
    xTaskCreate(ultrasonic_task1, "Ultrasonic Task 1", 1024, NULL, 1, NULL);
    xTaskCreate(ultrasonic_task2, "Ultrasonic Task 2", 1024, NULL, 1, NULL);
    xTaskCreate(motor_control_task, "Motor Task", 1024, NULL, 1, 0);
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    return 0;
}