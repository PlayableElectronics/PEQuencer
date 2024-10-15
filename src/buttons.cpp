#include "buttons.hpp"
#include "data_storage.hpp"
#include <EasyButton.h>
#include <RotaryEncoder.h>

constexpr int PIN_ROTA{18};
constexpr int PIN_ROTB{17};

int encoder_click_position{0};

EasyButton channel_buttons[8]{
    EasyButton(consts::led_pins[0], consts::debounce_value, true, false),
    EasyButton(consts::led_pins[1], consts::debounce_value, true, false),
    EasyButton(consts::led_pins[2], consts::debounce_value, true, false),
    EasyButton(consts::led_pins[3], consts::debounce_value, true, false),
    EasyButton(consts::led_pins[4], consts::debounce_value, true, false),
    EasyButton(consts::led_pins[5], consts::debounce_value, true, false),
    EasyButton(consts::led_pins[6], consts::debounce_value, true, false),
    EasyButton(consts::led_pins[7], consts::debounce_value, true, false)};

EasyButton menu_buttons[4]{
    EasyButton(consts::menu_led_pins[0], consts::debounce_value, true, false),
    EasyButton(consts::menu_led_pins[1], consts::debounce_value, true, false),
    EasyButton(consts::menu_led_pins[2], consts::debounce_value, true, false),
    EasyButton(consts::menu_led_pins[3], consts::debounce_value, true, false)};

EasyButton encoder_button(consts::switch_pin, consts::debounce_value, true,
                          false);

RotaryEncoder encoder(PIN_ROTB, PIN_ROTA, RotaryEncoder::LatchMode::FOUR3);
void buttons::update_buttons_state() {
  for (int i{0}; i < 4; ++i) {
    menu_buttons[i].read();
  }
  for (int i{0}; i < 8; ++i) {
    channel_buttons[i].read();
  }
  encoder_button.read();
}

void change_channel_state(const int i) { storage::channel = i; }

void change_channel_state() {}

namespace callbacks {
/* Menu buttons callbacks */
void set_menu_1() { storage::menu_position = 1; };
void set_menu_2() { storage::menu_position = 2; };
void set_menu_3() { storage::menu_position = 3; };
void set_menu_4() { storage::menu_position = 4; };

/* Channel buttons callbacks */
void set_channel_1() { storage::channel = 1; };
void set_channel_2() { storage::channel = 2; };
void set_channel_3() { storage::channel = 3; };
void set_channel_4() { storage::channel = 4; };
void set_channel_5() { storage::channel = 5; };
void set_channel_6() { storage::channel = 6; };
void set_channel_7() { storage::channel = 7; };
void set_channel_8() { storage::channel = 8; };

/* Rotary pin callback */
void set_rotary() { encoder.tick(); }

/* Encoder callback */
void encoder_callback() {
  storage::submenu_position += 1;
}
}; // namespace callbacks

void (*menu_callbacks[4])() = {callbacks::set_menu_1, callbacks::set_menu_2,
                               callbacks::set_menu_3, callbacks::set_menu_4};

void (*channel_callbacks[8])() = {
    callbacks::set_channel_1, callbacks::set_channel_2,
    callbacks::set_channel_3, callbacks::set_channel_4,
    callbacks::set_channel_5, callbacks::set_channel_6,
    callbacks::set_channel_7, callbacks::set_channel_8,
};

int buttons::get_rotary_encoder_position() { return encoder.getPosition(); }

void buttons::set_rotary_encoder_position(const int value) {
  encoder.setPosition(value);
}

void buttons::set_click_encoder_position(const int value) {
  encoder_click_position = value;
}

int buttons::get_click_encoder_position() { return encoder_click_position; }

void buttons::initialize() {
  pinMode(PIN_ROTA, INPUT_PULLUP);
  pinMode(PIN_ROTB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTA), callbacks::set_rotary, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTB), callbacks::set_rotary, CHANGE);

  for (int i{0}; i < 4; ++i) {
    menu_buttons[i].begin();
    menu_buttons[i].onPressed(menu_callbacks[i]);
  }
  for (int i{0}; i < 8; ++i) {
    channel_buttons[i].begin();
    channel_buttons[i].onPressed(channel_callbacks[i]);
  }
  encoder_button.begin();
  encoder_button.onPressed(callbacks::encoder_callback);
}
