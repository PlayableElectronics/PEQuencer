#include "main.hpp"
#include "pico/stdlib.h"
#include "tasks/task_display.hpp"
#include "tasks/task_hardware.hpp"
#include "tasks/task_sequencer.hpp"
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

static void init() {
  stdio_init_all();

  /* Enable stemma debug */
  stdio_uart_init_full(uart1, 115200, 20, 21);
}

static void init_tasks() {
  /* Create Queues */
  static QueueHandle_t xClock = xQueueCreate(1, sizeof(unsigned int));
  static QueueHandle_t xNote = xQueueCreate(1, sizeof(unsigned int[16]));

  /* Run tasks */
  xTaskCreate(task_clock, "clock", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(task_display, "display", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(task_sequencer, "sequencer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  vTaskStartScheduler();
}

int main() {
  init();
  init_tasks();
  return 0;
}
