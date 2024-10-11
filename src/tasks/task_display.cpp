#include "tasks/task_display.hpp"
#include "display.hpp"
#include <FreeRTOS.h>
#include <task.h>

void task_display(void *pv_parameters) {
  /* Initialize nullptr */
  BasicDisplay *display = nullptr;
  display = new MacropadDisplay();
  display->initialize();
  while (true) {
    display->clear();
    display->print();
    vTaskDelay(1);
  }
  delete display;
}
