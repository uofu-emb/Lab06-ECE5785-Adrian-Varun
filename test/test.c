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

SemaphoreHandle_t semaphore;

//=======================================

void setUp(void) {}

void tearDown(void) {}

//=======================================

void inv_task_a(void* params)
{
    while(1) {
    printf("Task A Waiting\n");
    xSemaphoreTake(semaphore, portMAX_DELAY);
    printf("Task A Busy\n");
    busy_wait_us(1000000);
    printf("Gave A Busy\n");
    xSemaphoreGive(semaphore);
    }
}

void inv_task_b(void* params)
{
    while(1)
    {
        printf("Task B Busy\n");
        busy_wait_us(100000);
    }
}

void inv_task_c(void* params)
{
    while(1) {
    printf("Task C Waiting\n");
    xSemaphoreTake(semaphore, portMAX_DELAY);
    printf("Task C Busy\n");
    busy_wait_us(100000);
    printf("Gave C Busy\n");
    xSemaphoreGive(semaphore);
    }
}

void activity_0()
{
    printf("Inversion test with  'xSemaphoreCreateBinary' \n");
    semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore);

    // Task A = Low Priority
    // Task B = Med Priority
    // Task C = High Priority    
    const int A_prior = 3;
    const int B_prior = 4;
    const int C_prior = 5;

    int A_stats, B_stats, C_stats, total_stats;

    task_manager(3, 
                inv_task_a, A_prior, &A_stats, 1, 
                inv_task_c, C_prior, &C_stats, 1, 
                inv_task_b, B_prior, &B_stats, 1, 
                &total_stats, NULL);
    
    printf("A: %i, B: %i, C: %i, total: %i \n", A_stats, B_stats, C_stats, total_stats);

    TEST_ASSERT(A_stats < 2000);
    TEST_ASSERT(B_stats > 2000);
    TEST_ASSERT(C_stats < 2000);

    vSemaphoreDelete(semaphore);
}

void activity_1()
{
    printf("Inversion test with  'xSemaphoreCreateMutex' \n");
    semaphore = xSemaphoreCreateMutex();
    xSemaphoreGive(semaphore);

    // Task A = Low Priority
    // Task B = Med Priority
    // Task C = High Priority    
    const int A_prior = 3;
    const int B_prior = 4;
    const int C_prior = 5;

    int A_stats, B_stats, C_stats, total_stats;

    task_manager(3, 
                inv_task_a, A_prior, &A_stats, 1, 
                inv_task_c, C_prior, &C_stats, 1, 
                inv_task_b, B_prior, &B_stats, 1, 
                &total_stats, NULL);
    
    printf("A: %i, B: %i, C: %i, total: %i \n", A_stats, B_stats, C_stats, total_stats);

    TEST_ASSERT(A_stats > 2000);
    TEST_ASSERT(B_stats < 2000);
    TEST_ASSERT(C_stats > 2000);

    vSemaphoreDelete(semaphore);
}

void monitor_task(__unused void* params)
{
    while (1)
    {
        RUN_TEST(activity_0);
        RUN_TEST(activity_1);
        vTaskDelay(1000);
    }
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    TaskHandle_t task;
    xTaskCreate(monitor_task, "MonitorThread",
                TASK_STACK_SIZE, NULL, 20, &task);
    vTaskStartScheduler();
    sleep_ms(5000);
    return UNITY_END();
}