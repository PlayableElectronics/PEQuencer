#include "main.hpp"
#include "data_store.hpp"
#include "tasks/task_display.hpp"
#include "tasks/task_hardware.hpp"
#include "tasks/task_sequencer.hpp"

void initSequencer() {
  stdio_init_all();
  stdio_uart_init_full(uart1, 115200, 20, 21);
}

int main() {
  initSequencer();
  QueueHandle_t xQueue = xQueueCreate(1, sizeof(dataStore *));
  xTaskCreate(task_display, "display", configMINIMAL_STACK_SIZE, (void *)xQueue, 1, NULL);
  xTaskCreate(task_hardware, "hardware", configMINIMAL_STACK_SIZE, (void *)xQueue, 1, NULL);
  xTaskCreate(task_sequencer, "sequencer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  vTaskStartScheduler();
}
