#include "tasks/task_display.hpp"
Adafruit_SH1106G display =
    Adafruit_SH1106G(128, 64, consts::OLED_MOSI, consts::OLED_CLK,
                     consts::OLED_DC, consts::OLED_RST, consts::OLED_CS);

dataStore *receivedData;

void drawText (const char* label, int labelValue, int yOffset) {
    display.fillRect(0, 0 + yOffset, 70, 9, SH110X_WHITE);
    display.setTextColor(SH110X_BLACK);
    display.setCursor(1, yOffset + 1);
    display.print(label);
    display.print(labelValue);
}

void task_display(void *pvParameters) {
  QueueHandle_t queue = (QueueHandle_t)pvParameters;
  display.begin(0, true);
  display.setTextSize(1);
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);
  display.clearDisplay();
  while (xQueueReceive(queue, &receivedData, portMAX_DELAY) == pdPASS) {
    display.clearDisplay();
    drawText("MENU_POS ", receivedData->menuPosition, 0);
    drawText("SMEN_POS ", receivedData->subMenuPosition, 10);
    drawText("CHAN_POS ", receivedData->channel, 20);
    drawText("ENCO_POS ", receivedData->encoderPosition, 30);
    display.display();
  }
  vTaskDelay(1);
}
