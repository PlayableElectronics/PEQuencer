#include "tasks/task_hardware.hpp"

const int BUTTONS[8] = {1, 2, 4, 5, 7, 8, 10, 11};
const int ACTION_BUTTONS[4] = {3, 6, 9, 12};
const int ENCODER[3] = {0, 18, 17}; // 1. Switch pin, 2. Pin Rotation A 3. Pin rotation B
int debounce = 5;

struct dataStore {
  int menuPosition;
  int subMenuPosition;
  int channel;
  int encoderPosition;
};
dataStore oldStore;
dataStore store;
dataStore* ptrOldStore = &oldStore;
dataStore* ptrStore = &store;

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

void encoderHandler() {
  encoder.tick();
  ptrStore->encoderPosition = encoder.getPosition();
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
  menuButtons[0].onPressed([]() { (ptrStore->menuPosition = 1); });
  menuButtons[1].onPressed([]() { (ptrStore->menuPosition = 2); });
  menuButtons[2].onPressed([]() { (ptrStore->menuPosition = 3); });
  menuButtons[3].onPressed([]() { (ptrStore->menuPosition = 4); });
  channelButtons[0].onPressed([]() { (ptrStore->channel = 1); });
  channelButtons[1].onPressed([]() { (ptrStore->channel = 2); });
  channelButtons[2].onPressed([]() { (ptrStore->channel = 3); });
  channelButtons[3].onPressed([]() { (ptrStore->channel = 4); });
  channelButtons[4].onPressed([]() { (ptrStore->channel = 5); });
  channelButtons[5].onPressed([]() { (ptrStore->channel = 6); });
  channelButtons[6].onPressed([]() { (ptrStore->channel = 7); });
  channelButtons[7].onPressed([]() { (ptrStore->channel = 8); });
  encoderButton.onPressed([]() { (ptrStore->subMenuPosition += 1); });
}

void updateDataStore () {
  // TODO: Create a loop that will send data to the FreeRTOS queue.
  // ref.: https://www.freertos.org/a00117.html
  // prints should be executed under #ifdef DEBUG
  if (ptrOldStore->menuPosition != ptrStore->menuPosition) {
    printf("NewMenuPosition: %d\n", ptrStore->menuPosition);
    ptrOldStore->menuPosition = ptrStore->menuPosition;
  }
  if (ptrOldStore->channel!= ptrStore->channel) {
    printf("New Channel Position: %d\n", ptrStore->channel);
    ptrOldStore->channel = ptrStore->channel;
  }
  if (ptrOldStore->subMenuPosition!= ptrStore->subMenuPosition) {
    printf("New SubMenu Position: %d\n", ptrStore->subMenuPosition);
    ptrOldStore->subMenuPosition = ptrStore->subMenuPosition;
  }
  if (ptrOldStore->encoderPosition!= ptrStore->encoderPosition) {
    printf("New SubMenu Position: %d\n", ptrStore->encoderPosition);
    ptrOldStore->encoderPosition= ptrStore->encoderPosition;
  }
}

void task_hardware(void *pvParameters) {
  pinMode(ENCODER[1], INPUT_PULLUP);
  pinMode(ENCODER[2], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER[1]), encoderHandler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER[2]), encoderHandler, CHANGE);
  beginButtons();
  handleCallbacks();
  while (true) {
    eventsRead();
    updateDataStore();
    vTaskDelay(1);
  }
}
