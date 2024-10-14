#include "channel.hpp"

Channel::Channel(int num, bool muted, int vel)
    : number(num), is_muted(muted), velocity(vel) {}

void Channel::set_velocity(const int new_velocity) {
  if (new_velocity >= 0 && new_velocity <= 127) {
    velocity = new_velocity;
  }
}

int Channel::get_velocity() const { return velocity; }
