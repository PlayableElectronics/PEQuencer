#include "tasks/task_hardware.hpp"
#include "data_storage.hpp"
#include <FreeRTOS.h>
#include <task.h>
#include <cstdio>

void task_sequence(void *pv_parameters) {
  const int channel_number = (int) pv_parameters;
  while (true) {
    printf("task number: %i, foo\n", channel_number);
    vTaskDelay(1);
  }
}
