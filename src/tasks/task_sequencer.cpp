#include "tasks/task_sequencer.hpp"
#include <FreeRTOS.h>
#include <cstdio>
#include <task.h>

void task_sequencer(void *pv_parameters) {
  while (true) {
    vTaskDelay(1);
  }
}
