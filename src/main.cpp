#include "defs.h"

// using namespace scale_;
// using namespace chord_;
// using namespace tone_;

// auto DSharpMajor = scale(Ds,scale_::major);

bool valueinarray(uint val, uint events[],
                  uint index) { // Check if element is on list
  bool out = false;
  for (int i = 0; i < index; i++) {
    if (val == events[i]) {
      out = true;
      break;
    }
  }
  return out;
}

void seq_calc(uint chan) {

  uint starts[16];
  uint ends[16];
  uint prev = 0;
  uint jndex = 0;
  int offset_temp_x = 0;
  uint index = 0;

  if (preset.tracks[chan].step == 0 || preset.tracks[chan].limit == 0) {
    score[chan][jndex++] = (byte)(127);
    score[chan][jndex++] = 0xE0;
  } else {
    if (preset.tracks[chan].gate == 0) {
      preset.tracks[chan].gate = 1;
    };
    for (int i = 0; i < 16; i++) {
      if (euc16[preset.tracks[chan].step][i] == 1) {
        offset_temp_x = i - preset.tracks[chan].offset;
        while (offset_temp_x > 0) {
          offset_temp_x = offset_temp_x - 16;
        }
        if (offset_temp_x < 0) {
          offset_temp_x = offset_temp_x * -1;
        }
        offset_temp_x = 16 - offset_temp_x;
        if (offset_temp_x == 16) {
          offset_temp_x = 0;
        };
        if (offset_temp_x < preset.tracks[chan].limit) {
          uint start = offset_temp_x * 32; // )+16;
          uint end = start + preset.tracks[chan].gate;
          if (start > 512)
            start = start - 512;
          if (end > 512)
            end = end - 512;
          starts[index] = start;
          ends[index] = end;
          index++;
        }
      }
    }

    uint delay = 0;
    uint prev_start = 0;
    uint prev_end = 0;
    uint check = 0;
    for (uint i = 0; i < preset.tracks[chan].limit * 32; i++) {
      if (valueinarray(i, starts, index) || valueinarray(i, ends, index)) {
        while (delay > 127) {
          score[chan][jndex++] = 0x7F;
          delay = delay - 127;
          check = check + 127 + 1;
        }
        if (delay != 0) {
          score[chan][jndex++] = (byte)(delay - 1);
          check = check + delay - 1;
        }
        delay = 1;
        if (valueinarray(i, starts, index)) {
          score[chan][jndex++] = (byte)(144 + chan); // Note ON
          // score[chan][jndex++] = 0xAA;               // Note Number
          score[chan][jndex++] =
              (byte)(preset.tracks[chan].note); // Note Number
          // score[chan][jndex++] = 0x41;               // Velocity
          score[chan][jndex++] =
              (byte)(preset.tracks[chan].velocity); // Velocity
        }
        if (valueinarray(i, ends, index) == 1) {
          score[chan][jndex++] = (byte)(128 + chan); // Note OFF
          // score[chan][jndex++] = 0xAA;               // Note Number
          score[chan][jndex++] =
              (byte)(preset.tracks[chan].note); // Note Number
        }
      }
      delay++;
    }
    while (delay > 127) {
      score[chan][jndex++] = 0x7F;
      delay = delay - 127;
      check = check + 127 + 1;
    }
    if (delay != 0) {
      score[chan][jndex++] = (byte)(delay - 1);
      check = check + delay - 1;
    }

    // printf(" \n Check: %i \n", check);
    score[chan][jndex++] = 0xE0;
  }

  // if(chan == 0){
  //   for(int i = 0 ;i < 32; i++){
  //     printf("0x%02x ",score[chan][i]);
  //   }
  //   printf("\n \n");
  // }
}
void clock_task(void *pvParameters) {
  gpio_init(13);
  gpio_set_dir(13, GPIO_OUT);
  // tbd groove
  uint step = 0;
  while (true) {
    xQueueSend(xClock, &step, 0U);
    if (step % 128 == 0) {
      // seq_calc(0);
      gpio_put(13, 1);
    } else {
      gpio_put(13, 0);
    }
    step++;
    if (step >= 128) {
      step = 0;
    }

    int bpm = 60.0 / preset.bpm * 1000.0;
    vTaskDelay(pdMS_TO_TICKS(bpm));
  }
}

void seq_task(void *pvParameters) {
  uint score_cursor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint wait_ticks[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  uint playing_step_32[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  // char lists [250];
  byte cmd, opcode, chan, note;
  for (int i = 0; i < 8; i++) {
    seq_calc(i); // recalc all
  }
  while (true) {
    // vTaskList(lists);
    //  printf("********************************** \n");
    //  printf("Task  State   Prio    Stack    Num \n");
    //  printf("********************************** \n");
    //  printf(lists);
    //  printf("\n");
    //  printf("********************************** \n");
    //  system("clear");

    // xQueueReceive(xClock, &step, portMAX_DELAY);
    // for(int i = 0; i < 8; i++){ //channels
    for (int i = 0; i < 8; i++) { // channels
      if (sizeof(score[i]) > 0) {
        if (wait_ticks[i] && --wait_ticks[i] == 0) {
          while (true) {
            cmd = score[i][score_cursor[i]++];
            if (i == 0)
              printf(" SC: %i , CMD: 0x%02x \n ", score_cursor[i], cmd);
            if (cmd < 0x80) { /* wait count in msec. */
              wait_ticks[i] = (unsigned)cmd;
              if (wait_ticks[i] == 0)
                wait_ticks[i] = 1;
              // printf("%i \n", wait_ticks[i]);
              break;
            }
            opcode = cmd & 0xf0;
            chan = cmd & 0x0f;
            if (opcode == CMD_STOPNOTE) { /* stop note */
              note = score[i][score_cursor[i]++];
              // score_cursor[i]++; // ignore it
              // printf("note off %i channel %i\n",note, chan);
              ccolors[channel_led[i]] = 0;
            } else if (opcode == CMD_PLAYNOTE) { /* play note */
              note = score[i][score_cursor[i]++];
              ++score_cursor[i]; // ignore volume
              // printf("note on %i channel %i\n",note, chan);
              ccolors[channel_led[i]] = 0x0c0c0c;
            } else if (opcode ==
                       CMD_INSTRUMENT) { /* change a channel's instrument */
              score_cursor[i]++;         // ignore it
            } else if (opcode == CMD_RESTART) { /* restart score */
              score_cursor[i] = 0;
              seq_calc(i); // recalc one
              playing_step[i] = -1;
              playing_step_32[i] = 0;
            }
          }
        }
        if (preset.tracks[chan].limit != 0) {
          if (playing_step_32[i] % 32 == 0) {
            playing_step[i]++;
          }
          playing_step_32[i]++;
        } else if (preset.tracks[chan].limit == 0) {
          playing_step[i] == 1;
        }
      }
      rgb_update();
    }
    int bpm = 60.0 / preset.bpm * 1000.0 / 32.0;
    vTaskDelay(pdMS_TO_TICKS(bpm));
  }
}

int main() {
  preset.bpm = 128;
  stdio_init_all();
  stdio_uart_init_full(uart1, 115200, 20, 21); // debug via stemma
  rgb_init();
  xClock = xQueueCreate(1, sizeof(uint));
  xNote = xQueueCreate(1, sizeof(uint[16]));
  // xTaskCreate(clock_task, "clock", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(display_task, "display", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(seq_task, "sequencer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  vTaskStartScheduler();
}
