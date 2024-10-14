#ifndef DATA_STORE_HPP
#define DATA_STORE_HPP

#include "channel.hpp"

namespace storage {
extern Channel channels[8];
extern int menu_position;
extern int submenu_position;
extern const char *names[4][6];
extern int channel;
} // namespace store

#endif
