#ifndef DATA_STORE_HPP
#define DATA_STORE_HPP

#include "channel.hpp"

namespace consts {
static const int led_pins[8]{1, 2, 4, 5, 7, 8, 10, 11};
static const int menu_led_pins[4]{3, 6, 9, 12};
static const int switch_pin{0};
static const int debounce_value{5};
} // namespace consts

namespace storage {
extern const char *names[4][6];
extern Channel channels[8];
extern int menu_position;
extern int submenu_position;
extern int channel;
} // namespace storage

#endif
