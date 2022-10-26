#include "defs.h"

struct Track {
  uint step;
  uint offset;
  uint limit;
  int  note;
  uint mute;
  uint gate;
  uint channel;
  uint velocity;
  float bpm;
};

struct Preset {
  struct Track tracks[16];
};

struct Preset preset2;

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
  //uint states[16];
  //struct Note tracks[16];
};

struct Note note;
struct NoteSet noteset;

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
    vTaskDelay(pdMS_TO_TICKS((60.0/preset2.tracks[0].bpm)*50.0));
  }
}

void seq_task(void *pvParameters){
    uint step;
    while(true){
        xQueueReceive(xClock, &step, portMAX_DELAY);
        struct NoteSet noteset;
        if(step==0){
            noteset.gates[0]=2;
            noteset.gates[1]=2;
            noteset.gates[2]=0;
            noteset.gates[3]=2;
            noteset.gates[4]=2;
            noteset.gates[5]=0;
            noteset.gates[6]=2;
            noteset.gates[7]=2;
            noteset.gates[8]=0;
            noteset.gates[9]=2;
            noteset.gates[10]=2;
            noteset.gates[11]=0;
        }
        for(int i =0;i<16;i++){
          if(noteset.gates[i]!=0){
            noteset.gates[i]--;
          }
        }
        xQueueSend(xNoteOn, &noteset, 0U);
    }
}

void noteon_task(void *pvParameters){
    struct NoteSet noteset;
    while(true){
        xQueueReceive(xNoteOn, &noteset, portMAX_DELAY);
        for(int i=0;i<12;i++){
          if(noteset.gates[i]!=0){
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
    preset2.tracks[0].bpm = 128;
    stdio_init_all();
    rgb_init();
    //task
    xClock = xQueueCreate(1, sizeof(uint));
    xNoteOn = xQueueCreate(1, sizeof(noteset));
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
    xTaskCreate(seq_task, "seq", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(noteon_task, "note", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(display_task, "display", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){
    };
}
