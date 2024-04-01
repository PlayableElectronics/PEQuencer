#include "tasks/task_hardware.hpp"

void task_hardware(void *pvParameters) {
  while (true) {
    Logger::info("task_hardware");
    vTaskDelay(1);
  }
}
