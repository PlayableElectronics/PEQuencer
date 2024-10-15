#include "tasks/task_display.hpp"
#include "Adafruit_SH110X.h"
#include "buttons.hpp"
#include "data_storage.hpp"
#include "display.hpp"
#include <FreeRTOS.h>
#include <task.h>

namespace display_utils {
void initialize(Adafruit_SH1106G *display) {
  display->begin(0, true);
  display->setTextSize(1);
  display->setTextColor(SH110X_BLACK, SH110X_WHITE);
  display->clearDisplay();
};

void clear(Adafruit_SH1106G *display) { display->clearDisplay(); }

void display(Adafruit_SH1106G *display) { display->display(); }
} // namespace display_utils

void draw_menu(Adafruit_SH1106G *display) {
  for (int i = 0; i < 4; ++i) {
    if (i + 1 != storage::menu_position)
      continue;
    for (int j = 0; j < 5; ++j) {
      if (storage::names[i][j] == nullptr) {
        storage::submenu_position = 1;
        continue;
      }
      if (storage::submenu_position > 5)
        storage::submenu_position = 1;
      /* Rectangle under inscription */
      display->setTextColor(SH110X_BLACK);
      if (storage::submenu_position == j + 1) {
        display->fillRect(0, j * 10, 31, 11, SH110X_BLACK);
        display->setTextColor(SH110X_WHITE);
      } else {
        display->fillRect(0, j * 10, 31, 11, SH110X_WHITE);
      }
      /* Menu name */
      display->setCursor(1, j * 10 + 2);
      display->print(storage::names[i][j]);
    }
  }
}

void draw_channel_state(Adafruit_SH1106G *display) {
  /* display channel number */
  display->setTextColor(SH110X_WHITE);
  display->setCursor(2, 55);
  display->print(storage::channel);
  display->setCursor(9, 55);
  if (storage::channels[storage::channel - 1].is_muted)
    display->print("MUTE");
}

void task_display(void *pv_parameters) {
  Adafruit_SH1106G display{128, 64, 27, 26, 24, 23, 22};
  Adafruit_SH1106G *display_ptr = &display;
  display_utils::initialize(display_ptr);
  buttons::initialize();
  while (true) {
    buttons::update_buttons_state();
    display_utils::clear(display_ptr);
    /* start displaying */
    draw_menu(display_ptr);
    draw_channel_state(display_ptr);
    /* end displaying */
    display_utils::display(display_ptr);
    vTaskDelay(1);
  }
}
