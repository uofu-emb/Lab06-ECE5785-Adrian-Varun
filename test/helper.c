#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "helper.h"

#define TEST_DURATION       5000
#define TASK_STACK_SIZE     2048

void task_manager (
        int num_tasks,
        TaskFunction_t pri_task, int pri_task_priority, int* pri_task_stats, int pri_delay,
        TaskFunction_t sec_task, int sec_task_priority, int* sec_task_stats, int sec_delay,
        TaskFunction_t ter_task, int ter_task_priority, int* ter_task_stats, int ter_delay,
        int* total_task_stats, TickType_t* total_ticks
        )
{   
    TaskHandle_t pri_task_handle, sec_task_handle, ter_task_handle;

    if(num_tasks >= 1)
    {
    // Start the primary task
    printf("Start task 1\n");
    vTaskDelay(pdMS_TO_TICKS(pri_delay));
    xTaskCreate(pri_task, "Primary Thread",
                TASK_STACK_SIZE, NULL, pri_task_priority, &pri_task_handle);
    }
    if(num_tasks >= 2)
    {
    // Start the secondary task
    printf("Start task 2\n");
    vTaskDelay(pdMS_TO_TICKS(sec_delay));
    xTaskCreate(sec_task, "Secondary Thread",
                TASK_STACK_SIZE, NULL, sec_task_priority, &sec_task_handle);
    }
    if(num_tasks >= 3)
    {
        // Start the secondary task
        printf("Start task 3\n");
        vTaskDelay(pdMS_TO_TICKS(ter_delay));
        xTaskCreate(ter_task, "Tertiary Thread",
                TASK_STACK_SIZE, NULL, ter_task_priority, &ter_task_handle);
    }

    vTaskDelay(pdMS_TO_TICKS(TEST_DURATION));

    // Get the statistics
    *total_task_stats = 0;
    if(num_tasks >= 1)
    {
        *pri_task_stats = ulTaskGetRunTimeCounter(pri_task_handle);
        *total_task_stats += *pri_task_stats;
        vTaskDelete(pri_task_handle);
    }
    if(num_tasks >= 2)
    {
        *sec_task_stats = ulTaskGetRunTimeCounter(sec_task_handle);
        *total_task_stats += *sec_task_stats;
        vTaskDelete(sec_task_handle);
    }
    if(num_tasks >= 3)
    {
        *ter_task_stats = ulTaskGetRunTimeCounter(ter_task_handle);
        *total_task_stats += *ter_task_stats;
        vTaskDelete(ter_task_handle);
    }
}