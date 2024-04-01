#include "tasks/task_hardware.hpp"
#include <functional>

const int BUTTONS[8] = {1, 2, 4, 5, 7, 8, 10, 11};
const int ACTION_BUTTONS[4] = {3, 6, 9, 12};
const int ENCODER[3] = {0, 18, 17}; // 1. Switch pin, 2. Pin Rotation A 3. Pin rotation B
int debounce = 5;

// TODO: Assign these values to global data storage, maybye create singleton class?
int subMenuPosition = 1;
int currentMenu = 1;
int currentChannel = 1;

EasyButton channelButtons[8] = {
    EasyButton(BUTTONS[0], debounce, true, false),
    EasyButton(BUTTONS[1], debounce, true, false),
    EasyButton(BUTTONS[2], debounce, true, false),
    EasyButton(BUTTONS[3], debounce, true, false),
    EasyButton(BUTTONS[4], debounce, true, false),
    EasyButton(BUTTONS[5], debounce, true, false),
    EasyButton(BUTTONS[6], debounce, true, false),
    EasyButton(BUTTONS[7], debounce, true, false)
};

EasyButton menuButtons[4] = {
    EasyButton(ACTION_BUTTONS[0], debounce, true, false),
    EasyButton(ACTION_BUTTONS[1], debounce, true, false),
    EasyButton(ACTION_BUTTONS[2], debounce, true, false),
    EasyButton(ACTION_BUTTONS[3], debounce, true, false)
};

RotaryEncoder encoder(ENCODER[2], ENCODER[1], RotaryEncoder::LatchMode::FOUR3);
EasyButton encoderButton(ENCODER[0], debounce, true, false);

void checkPosition() {
  encoder.tick();
}

void eventsRead() {
  for (int i = 0; i < sizeof(menuButtons) / sizeof(menuButtons[0]); ++i) {
    menuButtons[i].read();
  }
  for (int i = 0; i < sizeof(channelButtons) / sizeof(channelButtons[0]); ++i) {
    channelButtons[i].read();
  }
  encoderButton.read();
}

void beginButtons() {
  for (int i = 0; i < sizeof(menuButtons) / sizeof(menuButtons[0]); ++i) {
    menuButtons[i].begin();
  }
  for (int i = 0; i < sizeof(channelButtons) / sizeof(channelButtons[0]); ++i) {
    channelButtons[i].begin();
  }
  encoderButton.begin();
}

void handleCallbacks() {
  menuButtons[0].onPressed([]() { (currentMenu = 1); });
  menuButtons[1].onPressed([]() { (currentMenu = 2); });
  menuButtons[2].onPressed([]() { (currentMenu = 3); });
  menuButtons[3].onPressed([]() { (currentMenu = 4); });
  channelButtons[0].onPressed([]() { (currentChannel = 1); });
  channelButtons[1].onPressed([]() { (currentChannel = 2); });
  channelButtons[2].onPressed([]() { (currentChannel = 3); });
  channelButtons[3].onPressed([]() { (currentChannel = 4); });
  channelButtons[4].onPressed([]() { (currentChannel = 5); });
  channelButtons[5].onPressed([]() { (currentChannel = 6); });
  channelButtons[6].onPressed([]() { (currentChannel = 7); });
  channelButtons[7].onPressed([]() { (currentChannel = 8); });
  encoderButton.onPressed([]() { (subMenuPosition += 1); });
}

void task_hardware(void *pvParameters) {
  pinMode(ENCODER[1], INPUT_PULLUP);
  pinMode(ENCODER[2], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER[1]), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER[2]), checkPosition, CHANGE);

  beginButtons();
  handleCallbacks();

  while (true) {
    eventsRead();
    printf("Encoder value -> %ld \n", encoder.getPosition());
    printf("Encoder click -> %ld \n", subMenuPosition);
    printf("Menu value ->: %d\n", currentMenu);
    printf("Channel value ->: %d\n", currentChannel);
    vTaskDelay(1);
  }
}
