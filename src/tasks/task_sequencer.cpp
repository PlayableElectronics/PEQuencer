#include "tasks/task_sequencer.hpp"

void task_sequencer(void *pvParameters) {
  while (true) {
    vTaskDelay(1);
  }
}
