#ifndef DATA_STORE_HPP
#define DATA_STORE_HPP
#include "channel.hpp"

namespace storage {
extern const char *names[4][6];
extern Channel channels[8];
extern int menu_position;
extern int submenu_position;
extern int channel;
extern int encoder_positions[4][5];
} // namespace storage

#endif
