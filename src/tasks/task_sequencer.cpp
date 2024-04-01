#include "tasks/task_sequencer.hpp"

void task_sequencer(void *pvParameters) {
  while (true) {
    Logger::info("task_sequencer");
    vTaskDelay(1);
  }
}
