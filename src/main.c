#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

#define TASK_STACK_SIZE     2048
#define HIGH_TASK_PRIORITY  1
#define LOW_TASK_PRIORITY   5

int main( void )
{
    stdio_init_all();
    return 0;
}
