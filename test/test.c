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


void busy_busy(void* params)
{
    for (int i = 0; ; i++);
}

void busy_yield(void* params)
{
    for (int i = 0; ; i++) {
        taskYIELD();
    }
}

void dummy_task(void* params){
    /** Since our runner function takes 3 arguments to run,
     * we define this dummy function to pass through but this function never executes.
     * We use number of tasks to be run as an input first before creating tasks.
     */
}


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

void activity_run_busy_busy(){
    int priority_level = tskIDLE_PRIORITY+(3);
    int A_stats, B_stats, dummy_stats, total_stats;
    task_manager(2, 
                busy_busy, priority_level, &A_stats, 1, 
                busy_busy, priority_level, &B_stats, 1, 
                dummy_task, priority_level, &dummy_stats, 1, 
                &total_stats, NULL);
    printf("A: %i, B: %i, total: %i \n", A_stats, B_stats, total_stats);
    TEST_ASSERT(A_stats > 2000000);
    TEST_ASSERT(B_stats > 2000000);
}

void monitor_task(__unused void* params)
{
    while (1)
    {
        RUN_TEST(activity_0);
        RUN_TEST(activity_1);
        RUN_TEST(activity_run_busy_busy);
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