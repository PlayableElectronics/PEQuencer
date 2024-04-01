#include "tasks/task_display.hpp"

void task_display(void *pvParameters) {
  while (true) {
    Logger::info("task_display");
    vTaskDelay(1);
  }
}
