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


#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE


void setUp(void) {}

void tearDown(void) {}

void inversion_test()
{
    //Sample Test, Not Actual Code
    TEST_ASSERT_EQUAL_INT(1, 1);
}

void monitor_task(__unused void* params)
{
    RUN_TEST(inversion_test);
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();

    TaskHandle_t task_1;
    printf("before task\n");
    xTaskCreate(monitor_task, "MonitorThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task_1);
    printf("after task\n");
    vTaskStartScheduler();

    sleep_ms(5000);
    return UNITY_END();
}