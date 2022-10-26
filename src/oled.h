#include "ssd1306.h"

void oled_task() {
    ssd1306_t disp;
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c_default);
    ssd1306_clear(&disp);

    while(1){
      int step;
      char str[10];
      xQueueReceive(xClock, &step, portMAX_DELAY);
      ssd1306_clear(&disp);
      sprintf(str, "%i", step);
      ssd1306_draw_string(&disp, 8, 24, 1, str);
      ssd1306_show(&disp);
    };
}
