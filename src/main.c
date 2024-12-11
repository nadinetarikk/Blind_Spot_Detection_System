#include "main.h"
// Global variables to store sensor readings (can be shared among tasks)
float ultrasonic1_distance = 0.0;
float ultrasonic2_distance = 0.0;
float ldr_intensity = 0;
bool is_SystemOn=false;
void ultrasonic_task1(void *params)
{
    printf("Ultrasonic Task 1\n");
    while (true)
    {
        printf("Ultrasonic Task 1\n");
        send_trigger_pulse();
        ultrasonic1_distance = calculate_distance(measure_echo_time());
        printf("Distance 1 : %f\n", ultrasonic1_distance);
        if (ultrasonic1_distance <= 3 && is_SystemOn)
        {
            if (ldr_intensity < 40)
            {
                pico_set_led(20, true); // Turn on LED 1
                buzzer_off();
            }
            else
            {
                pico_set_led(20, false);
                buzzer_beep(5000, 100); // Turn on Buzzer 1

            }
        }
        else
        {
            buzzer_off();
            pico_set_led(20, false); // Turn off LED 1
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Wait 100ms
    }
}

void ultrasonic_task2(void *params)
{
    printf("Ultrasonic Task 2\n");
    while (true)
    {
        printf("Ultrasonic Task 2\n");
        send_trigger_pulse2();
        ultrasonic2_distance = calculate_distance(measure_echo_time2());
        printf("Distance 2 : %f\n", ultrasonic2_distance);

        if (ultrasonic2_distance <= 3 && is_SystemOn)
        {
            if (ldr_intensity < 40)
            {
                pico_set_led(21, true); // Turn on LED 2
                buzzer_off2();
            }
            else
            {
                pico_set_led(21, false);
                buzzer_beep2(5000, 100); // Turn on Buzzer 2
            }
        }
        else
        {
            buzzer_off2();
            pico_set_led(21, false); // Turn off LED 2
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Wait 100ms
    }
}

void ldr_sensor_task(void *pvParameters)
{
    printf("LDR Task\n");
    while (1)
    {
        printf("LDR Task\n");
        uint16_t raw_value = read_ldr_gl5528_raw();
        float voltage = ldr_gl5528_to_voltage(raw_value);
        float resistance = ldr_gl5528_to_resistance(voltage);
        ldr_intensity = ldr_gl5528_to_light_intensity(resistance);

        vTaskDelay(300 / portTICK_PERIOD_MS); // Run every 300ms
    }
}

void motor_control_task(void *pvParameters)
{
    printf("Motor Task\n");
    while (1)
    {
        printf("Motor Task\n");
        if(is_SystemOn){
            if (!infrared_sensor_is_triggered() )
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
        }
        else
            {
                motor_stop(FRONT_LEFT);
                motor_stop(BACK_LEFT);
                motor_stop(FRONT_RIGHT);
                motor_stop(BACK_RIGHT);
            }
       
        
        vTaskDelay(pdMS_TO_TICKS(100)); // Run every 100ms
    }
}
void switch_task(void *pvParameters){
    while (1)
    {
        is_SystemOn=get_status();
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
    switch_init();
    // Create tasks
    printf("Starting tasks...\n");
    
    xTaskCreate(ldr_sensor_task, "LDR Task", 1024, NULL, 1, NULL);
    xTaskCreate(ultrasonic_task1, "Ultrasonic Task 1", 1024, NULL, 1, NULL);
    xTaskCreate(ultrasonic_task2, "Ultrasonic Task 2", 1024, NULL, 1, NULL);
    xTaskCreate(motor_control_task, "Motor Task", 1024, NULL, 1, 0);
    xTaskCreate(switch_task,"Switch Taske",1024,NULL,2,0);
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    return 0;
}