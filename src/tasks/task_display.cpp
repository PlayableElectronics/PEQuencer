#include "tasks/task_display.hpp"
#include "Adafruit_SH110X.h"
#include "data_store.hpp"
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

namespace menu_utils {
void draw_menu(Adafruit_SH1106G *display) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (menu::names[i][j] != nullptr) {
        // debug printf("%s\n", menu::names[i][j]);
        /* Rectangle under inscription */
        display->setTextColor(SH110X_BLACK);
        if (menu::submenu_position == j + 1) {
          display->fillRect(0, j * 10, 31, 11, SH110X_BLACK);
          display->setTextColor(SH110X_WHITE);
        } else {
          display->fillRect(0, j * 10, 31, 11, SH110X_WHITE);
        }
        /* Menu name */
        display->setCursor(1, j * 10 + 2);
        display->print(menu::names[i][j]);

        printf("menu::menu_position: %i\n", menu::menu_position);
        printf("menu::channel: %i\n", menu::channel);
        printf("channel::channels[menu::channel-1].number: %i\n", channel::channels[2].number);

      } else {
      }
    }
  }
}

void draw_channel_state(Adafruit_SH1106G *display) {}

} // namespace menu_utils

void task_display(void *pv_parameters) {
  Adafruit_SH1106G display{128, 64, 27, 26, 24, 23, 22};
  Adafruit_SH1106G *display_ptr = &display;
  display_utils::initialize(display_ptr);
  while (true) {
    display_utils::clear(display_ptr);
    /* start displaying */
    menu_utils::draw_menu(display_ptr);
    menu_utils::draw_channel_state(display_ptr);
    /* end displaying */
    display_utils::display(display_ptr);
    vTaskDelay(1);
  }
}
