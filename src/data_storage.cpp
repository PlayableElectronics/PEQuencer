#include "data_storage.hpp"

const char *storage::names[4][6]{{"STEP", "OFF", "LIMIT", "NOTE", "BPM"},
                                 {"CHAN", "GATE", "VELO", "EACH", "SWING"},
                                 {"SIGN", nullptr, nullptr, nullptr, nullptr},
                                 {nullptr, nullptr, nullptr, nullptr, nullptr}};

int storage::menu_position{1};
int storage::submenu_position{1};
int storage::channel{1};

Channel storage::channels[8]{Channel(1), Channel(2), Channel(3), Channel(4),
                             Channel(5), Channel(6), Channel(7), Channel(8)};

int storage::encoder_positions[4][5]{
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
