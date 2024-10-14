#ifndef BUTTONS_HPP
#define BUTTONS_HPP

namespace buttons {
extern void update_buttons_state();
extern void initialize();
extern int get_click_encoder_position();
extern int get_rotary_encoder_position();
extern void set_click_encoder_position(const int value);
extern void set_rotary_encoder_position(const int value);
} // namespace buttons
#endif
