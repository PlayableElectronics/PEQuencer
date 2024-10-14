#include "data_store.hpp"

const char *menu::names[4][6] = {{"STEP", "OFF", "LIMIT", "NOTE", "BPM"},
                                 {"CHAN", "GATE", "VELO", "EACH", "SWING"},
                                 {"SIGN", nullptr, nullptr, nullptr, nullptr},
                                 {nullptr, nullptr, nullptr, nullptr, nullptr}};

int menu::menu_position = 1;
int menu::submenu_position = 1;
int menu::channel = 1;

Channel channel::channels[8] = {Channel(1), Channel(2), Channel(3), Channel(4),
                       Channel(5), Channel(6), Channel(7), Channel(8)};
