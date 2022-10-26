#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "rgb.h"
//#include "oled.h"
#include "ui/ui.h"

struct Track {
  uint step;
  uint offset;
  uint limit;
  int  note;
  uint mute;
  uint gate;
  uint channel;
  uint velocity;
  uint bpm;
};

struct Preset {
  struct Track tracks[16];
};

struct Track track;
struct Preset preset2;

const uint LED_PIN = 13;//PICO_DEFAULT_LED_PIN;
static QueueHandle_t xClock, xSeq, xNoteOn  = NULL;

//SemaphoreHandle_t xSemaphoreUI;

void clock_task(void *pvParameters) {
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  //tbd groove
  int step = 0;
  while (true) {
    xQueueSend(xClock, &step, 0U);
    if (step%16==0){
      gpio_put(LED_PIN, 1);
    }
    else{
      gpio_put(LED_PIN, 0);
    }
    step++;
    if (step>=16) step=0;
    vTaskDelay(pdMS_TO_TICKS(30));
  }
}

void seq_task(void *pvParameters){
    uint step;
    uint notes[16];
    uint seq[16][16]={
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    while(true){
        xQueueReceive(xClock, &step, portMAX_DELAY);
        printf("%i\n", step);
        notes[0]=1;
        xQueueSend(xNoteOn, &notes, 0U);
        //noteset.gates=seq[step];
        /*
        if(step==0){
            noteset.gates[0]=1;
            printf("foo\n");
        }
        if(step==1){
            noteset.gates[0]=0;
            printf("bar\n");
        }*/
        //if(seq[step]!=0){
        //  for(int i =0;i<16;i++){
        //      noteset.leds[i]=(seq[step] >> 15-i) & 1;
        //  }
        //}
    }
}

void noteon_task(void *pvParameters){
    uint notes[16];
    while(true){
        xQueueReceive(xNoteOn, &notes, portMAX_DELAY);
        for(int i=0;i<12;i++){
          if(notes[i]==1){
            put_pixel(0x0c0c0c);
          }
          else {
            put_pixel(0);
          }
        }
    }
}
/*
void noteoff_task(void *pvParameters){
    struct Note note;
    while(true){
        xQueueReceive(xNoteOff, &note, 1000);
        set_pixel(note.channel, 0);
    }
}*/

/*
void ch1_task(void *pvParameters){
    while(true){
        if(xSemaphoreTake(ch1, (TickType_t) 10) == pdTRUE){
          ccolors[0] = 0xFFFFFF;
          vTaskDelay(10);
          ccolors[0] = 0;
        }
    }
}

void ch2_task(void *pvParameters){
    while(true){
        if(xSemaphoreTake(ch2, (TickType_t) 10) == pdTRUE){
          ccolors[1] = 0xFFFFFF;
          vTaskDelay(10);
          ccolors[1] = 0;
        }
    }
}*/

int main() {
    stdio_init_all();
    rgb_init();
    //task
    uint notes[16];
    xClock = xQueueCreate(1, sizeof(uint));
    //xSemaphoreUI = xSemaphoreCreateBinary();
    //xNoteOn = xQueueCreate(1, sizeof(notes));
    //xSeq = xQueueCreate(1, 256);
    //xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    //xTaskCreate(oled_task, "OLED_Task", 256, NULL, 1, NULL);
    //xTaskCreate(channel_task, "RGB_Task1", 256, (void *) ch1, 1, NULL);
    //xTaskCreate(channel_task, "RGB_Task2", 256, (void *) ch3, 1, NULL);
    //xTaskCreate(send_task, "send", 256, NULL, 1, NULL);
    //xTaskCreate(send_task1, "send", 256, NULL, 1, NULL);
    xTaskCreate(clock_task, "clock", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    //xTaskCreate(seq_task, "seq", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    //xTaskCreate(noteon_task, "note", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    //xTaskCreate(noteoff_task, "noteoff", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    //xTaskCreate(note1_task, "note1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    //xTaskCreate(ch2_task, "note1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(display_task, "display", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){
    };
}
