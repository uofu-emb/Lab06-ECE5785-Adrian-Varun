// #include <stdio.h>

// #include <unity.h>
// #include "unity_config.h"

// #include "FreeRTOS.h"
// #include "task.h"
// #include "semphr.h"

// #include "pico/stdlib.h"
// #include "pico/multicore.h"

// #include "helper.h"

// #define TASK_STACK_SIZE 2048


// #define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
// #define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE


// void setUp(void) {}

// void tearDown(void) {}

// void inversion_test()
// {
//     //Sample Test, Not Actual Code
//     printf("before\n");
//     TEST_ASSERT_EQUAL_INT(1, 1);
//     printf("after\n");
// }

// void monitor_task(__unused void* params)
// {

//     TEST_ASSERT_EQUAL_INT(1, 1);
//     //inversion_test();
// }

// int main (void)
// {
//     stdio_init_all();
//     sleep_ms(5000); // Give time for TTY to attach.
//     printf("Start tests\n");
//     UNITY_BEGIN();

//     TaskHandle_t task_1;
//     printf("before task\n");
//     xTaskCreate(monitor_task, "MonitorThread",
//                 MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task_1);
//     printf("after task\n");
//     vTaskStartScheduler();

//     sleep_ms(5000);
//     return UNITY_END();
// }


/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

int count = 0;
bool on = false;

#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void blink_task(__unused void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        if (count++ % 11) on = !on;
        vTaskDelay(500);
    }
}

void main_task(__unused void *params) {
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
    char c;
    while(c = getchar()) {
        if (c <= 'z' && c >= 'a') putchar(c - 32);
        else if (c >= 'A' && c <= 'Z') putchar(c + 32);
        else putchar(c);
    }
}

int main( void )
{
    stdio_init_all();
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
    vTaskStartScheduler();
    return 0;
}