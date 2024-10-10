#include "tasks/task_hardware.hpp"
#include <FreeRTOS.h>
#include <task.h>

void task_clock(void *pv_parameters) {
  while (true) {
    vTaskDelay(1);
  }
}
