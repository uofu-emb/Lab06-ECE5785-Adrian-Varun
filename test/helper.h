#pragma once

void task_manager (
        int num_tasks,
        TaskFunction_t pri_task, int pri_task_priority, int* pri_task_stats, int pri_delay,
        TaskFunction_t sec_task, int sec_task_priority, int* sec_task_stats, int sec_delay,
        TaskFunction_t ter_task, int ter_task_priority, int* ter_task_stats, int ter_delay,
        int* total_task_stats, TickType_t* total_ticks
        );

