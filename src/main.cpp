#include "defs.h"

void clock_task(void *pvParameters) {
  gpio_init(13);
  gpio_set_dir(13, GPIO_OUT);
  //tbd groove
  int step = 0;
  while (true) {
    xQueueSend(xClock, &step, 0U);
    if (step%16==0){
      gpio_put(13, 1);
    }
    else{
      gpio_put(13, 0);
    }
    step++;
    if (step>=256) step=0;
    vTaskDelay(pdMS_TO_TICKS((60.0/preset.tracks[0].bpm)*60.0));
  }
}

void step_task(void *pvParameters){
    uint step;
    uint gates[16];
    while(true){
        xQueueReceive(xClock, &step, portMAX_DELAY);
        for(int i =0;i<16;i++){
          if (offset_buf[i][playing_step[i]] == 1 && preset.tracks[i].mute == 1) {
            gates[i] = preset.tracks[i].gate+1;
          }
          if (step%16==0){
            playing_step[i]++;
            if (playing_step[i] >= preset.tracks[i].limit) {
                playing_step[i] = 0;
            }
          }
          if(gates[i]!=0) gates[i]--; //decrement gates
        }
        xQueueSend(xNote, &gates, 0U);
        rgb_update();
    }
}

void note_task(void *pvParameters){
    uint channel_led[8] = {0,1,3,4,6,7,9,10};
    uint gates[16];
    while(true){
        xQueueReceive(xNote, &gates, portMAX_DELAY);
        for(int i=0;i<8;i++){
          if(gates[i]!=0){
            if(i==select_ch-1 ){
              ccolors[channel_led[i]] = 0xffffff;
            }
            else{
              ccolors[channel_led[i]] = 0x0c0c0c;
            }
          }
          else {
            ccolors[channel_led[i]] = 0;
          }
        }
    }
}

int main() {
    preset.tracks[0].bpm = 128;
    stdio_init_all();
    stdio_uart_init_full(uart1, 115200, 20, 21); //debug via stemma
    rgb_init();
    xClock = xQueueCreate(1, sizeof(uint));
    xNote = xQueueCreate(1, sizeof(uint[16]));
    xTaskCreate(clock_task, "clock", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(step_task, "step", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(note_task, "note", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(display_task, "display", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}
