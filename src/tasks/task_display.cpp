#include "tasks/task_display.hpp"
#include <FreeRTOS.h>
#include <task.h>

void task_display(void *pv_parameters) {
  while (true) {
    vTaskDelay(1);
  }
}
