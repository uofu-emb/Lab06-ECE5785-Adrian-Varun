#include <stdio.h>

#include <unity.h>
#include "unity_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "helper.h"

#define TASK_STACK_SIZE 2048





void setUp(void) {}

void tearDown(void) {}

void inversion_test()
{

}

void monitor_task(void* params)
{
    inversion_test();
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();

    TaskHandle_t monitor_task;
    xTaskCreate(monitor_task, "Monitor Thread",
                TASK_STACK_SIZE, NULL, 0, NULL);

    sleep_ms(5000);
    return UNITY_END();
}
