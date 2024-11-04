#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

#define TASK_STACK_SIZE     2048

void task_manager (
        TaskFunction_t pri_task, int pri_task_priority, int* pri_task_stats, int pri_delay,
        TaskFunction_t sec_task, int sec_task_priority, int* sec_task_stats, int sec_delay,
        int* total_task_stats, TickType_t* total_ticks
        )
{   
    TaskHandle_t pri_task_handle, sec_task_handle;

    // Start the primary task
    vTaskDelay(pri_delay);
    xTaskCreate(pri_task, "Primary Thread",
                TASK_STACK_SIZE, NULL, pri_task_priority, &pri_task_handle);

    // Start the secondary task
    vTaskDelay(sec_delay);
    xTaskCreate(sec_task, "Secondary Thread",
                TASK_STACK_SIZE, NULL, sec_task_priority, &sec_task_handle);

    // Get the statistics
    *pri_task_stats = ulTaskGetRunTimeCounter();

    vTaskDelete(pri_task_handle);
    vTaskDelete(sec_task_handle);
}