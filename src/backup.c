#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "rgb.h"
#include "hardware/i2c.h"
#include "oled.h"
//#include "ui.h"

#define I2C_SDA_PIN 31
#define I2C_SCL_PIN 32

struct Note {
  uint state;
  uint channel;
  uint pitch;
  int  shift;
  uint velocity;
  uint length;
};

struct NoteSet {
  uint gates[16];
  uint states[16];
  //struct Note tracks[16];
};

struct Note note;
struct NoteSet noteset;

const uint LED_PIN = 13;//PICO_DEFAULT_LED_PIN;
static QueueHandle_t xClock, xNoteOn, xNoteOff  = NULL;

void clock_task() {
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
/*
void seq_task(void *pvParameters){
    int step;
    struct Note note;
    struct NoteSet noteset;
    while(true){
        xQueueReceive(xClock, &step, portMAX_DELAY);
        if(step==0){
            noteset.gates[0]=1;
            printf("foo\n");
        }
        if(step==8){
            noteset.gates[0]=0;
            printf("bar\n");
        }
        //if(seq[step]!=0){
        //  for(int i =0;i<16;i++){
        //      noteset.leds[i]=(seq[step] >> 15-i) & 1;
        //  }
        //}
        xQueueSend(xNoteOn, &noteset, 0U);
    }
}

void noteon_task(void *pvParameters){
    struct NoteSet noteset;
    struct NoteSet activeset;
    while(true){
        xQueueReceive(xNoteOn, &noteset, portMAX_DELAY);
        for(int i=0;i<12;i++){
          if(noteset.gates[i]==1){
            put_pixel(0x0c0c0c);
            noteset.states[i]=1;
          }
          else if(noteset.states[i]==1){
            put_pixel(0);
          }
        }
    }
}

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

    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    rgb_init();


    //task
    xClock = xQueueCreate(1, sizeof(uint));
    xNoteOn = xQueueCreate(1, sizeof(noteset));
    xNoteOff = xQueueCreate(1, sizeof(note));
    //xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(oled_task, "OLED_Task", 256, NULL, 1, NULL);
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
    //xTaskCreate(led_task, "led", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){
    };
}
