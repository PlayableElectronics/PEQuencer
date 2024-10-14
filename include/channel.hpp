#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel {
public:
  Channel(int num, bool muted = true, int vel = 100);
  int get_velocity() const;
  void set_velocity(const int new_velocity);
  bool is_muted;
  int number;
private:
  int velocity;
};

#endif
