#include "defs.h"

// using namespace scale_;
// using namespace chord_;
// using namespace tone_;

//auto DSharpMajor = scale(Ds,scale_::major);

void seq_calc(uint chan){
  uint starts[16];
  int offset_temp_x = 0;
  uint index = 0;

  for(int i = 0; i < 16 ;i++){
    //score[chan][i]=0;
    if (euc16[preset.tracks[chan].step][i] == 1){
        offset_temp_x = i - preset.tracks[chan].offset;
        while (offset_temp_x > 0){offset_temp_x = offset_temp_x - 16;}
        if (offset_temp_x < 0){offset_temp_x = offset_temp_x * -1;}
        offset_temp_x = 16 - offset_temp_x;
        if (offset_temp_x == 16){offset_temp_x = 0;};
        if (offset_temp_x < preset.tracks[chan].limit){
          starts[index]=offset_temp_x;
          index++;
        }      
    } 
  }

  uint starts_sorted[index];

  for(int i = 0; i < index; i++){
    starts_sorted[i] = starts[i];
  }
  
  int lt_length = sizeof(starts_sorted) / sizeof(starts_sorted[0]);
  qsort(starts_sorted, lt_length, sizeof(starts_sorted[0]), sort_asc);

  uint scores[32];
  int prev = 0;
  
  int gate = preset.tracks[chan].gate;
  if (gate == 0){gate = 1;};

  uint jndex = 0;
  for(int i = 0 ; i < index; i++){
    if (preset.tracks[chan].step == 0){ // If 0 Steps Exit
      score[chan][jndex++] = (byte)(224);
      break;
    }
    if (prev == 0){ // Only for first step || Delay calculation
      if ((starts_sorted[0] * 32 + 16) > 127 and (starts_sorted[0] * 32 + 16) <= 255){
        score[chan][jndex++] = (byte)(127);
        //printf("0x%02x ", (byte)(127));
        score[chan][jndex++] = (byte)(starts_sorted[0] * 32 + 16 - 127);
        //printf("0x%02x ", (byte)(starts_sorted[0] * 32 + 16 - 127));
      } else if((starts_sorted[0] * 32 + 16) > 255){
        score[chan][jndex++] = (byte)(127);
        //printf("0x%02x ", (byte)(127));
        score[chan][jndex++] = (byte)(127);
        //printf("0x%02x ", (byte)(127));
        score[chan][jndex++] = (byte)(starts_sorted[0] * 32 + 16 - 127);
        //printf("0x%02x ", (byte)(starts_sorted[0] * 32 + 16 - 127));
      }
      else {
        score[chan][jndex++] = (byte)(starts_sorted[0] * 32 + 16);
        //printf("0x%02x ",(byte)(starts_sorted[0] * 32 + 16));
      }

    }
    prev++;

    score[chan][jndex++] = 0x90;
    //printf("0x%02x ", 0x90); // Note ON
    score[chan][jndex++] = 0xAA;
    //printf("0x%02x ", 0xAA); // Note Number
    score[chan][jndex++] =  0x41;
    //printf("0x%02x ", 0x41); // Note Velocity
    score[chan][jndex++] =  (byte)(gate);
    //printf("0x%02x ", gate); // Gate
    score[chan][jndex++] =  0x80;
    //printf("0x%02x ", 0x80);  // Note OFF
    score[chan][jndex++] =  0xAA;
    //printf("0x%02x ", 0xAA);  // Note Number

    if ((starts_sorted[i + 1] - starts_sorted[i]) * 32 > 127){
      score[chan][jndex++] =  (byte)(127);
      //printf("0x%02x ",(byte)(127)); // Delay
      score[chan][jndex++] =  (byte)((starts_sorted[i + 1] - starts_sorted[i]) * 32 - 127);
      //printf("0x%02x ",(byte)((starts_sorted[i + 1] - starts_sorted[i]) * 32 - 127));  // Delay
    }
    else{
      score[chan][jndex++] = (byte)((starts_sorted[i + 1] - starts_sorted[i]) * 32);
      //printf("0x%02x ",(byte)((starts_sorted[i + 1] - starts_sorted[i]) * 32));  // Delay
    }
    if (prev == index){
      score[chan][jndex++] = (byte)(((preset.tracks[chan].limit - starts_sorted[index - 1])-1)*32);
      //printf("0x%02x ",(byte)(((preset.tracks[chan].limit - starts_sorted[index - 1])-1)*32)); // End Delay
      score[chan][jndex++] = (byte)(224);
      //printf("0x%02x ",(byte)(224)); // End Pattern
    }
  }
  printf("\n");
  if(chan==0){
    for(int i =0;i<32;i++){
      printf("0x%02x ",score[chan][i]);
    }
    printf("\n");
  }
}


// void seq_calc(uint chan){
//   uint starts[16];
//   uint ends[16];
//   uint prev = 0;
//   int offset_temp_x = 0;
//   uint index = 0;

//   for(int i = 0; i < 16 ;i++){
//     //score[chan][i]=0;
//     if (euc16[preset.tracks[chan].step][i] == 1){
//         offset_temp_x = i - preset.tracks[chan].offset;
//         while (offset_temp_x > 0){offset_temp_x = offset_temp_x - 16;}
//         if (offset_temp_x < 0){offset_temp_x = offset_temp_x * -1;}
//         offset_temp_x = 16 - offset_temp_x;
//         if (offset_temp_x == 16){offset_temp_x = 0;};
//         //uint start = (offset_temp_x*32)+16;
//         if (offset_temp_x < preset.tracks[chan].limit){
//           uint start = (offset_temp_x*32)+16;
//           //start = start + preset.tracks[chan].stepOffset[i];
//           uint end = start + preset.tracks[chan].gate;
//           if (start>512) start = start-512;
//           if (end>512) end = end-512;
//           starts[index]=start;
//           ends[index]=end;
//           // printf("%i ", (offset_temp_x*32)+16);
//           index++;
//         }      
//     } 
//   }


//   //uint starts_sorted[index];
//   //uint ends_sorted[index];
//   uint events[index*2];

//   for(int i =0 ; i < index; i++){
//     events[i] = starts[i];
//     events[i + index] = ends[i]; 
//   }

//   //memcpy(starts_sorted, starts, sizeof(starts_sorted));
//   //memcpy(ends_sorted, ends, sizeof(ends_sorted));

//   //starts = (uint *) realloc(starts, index);

//   //qsort(starts_sorted, index, sizeof(starts_sorted[0]), sort_asc);
//   //qsort(ends_sorted, index, sizeof(ends_sorted[0]), sort_asc);

//   qsort(events, index * 2, sizeof(events[0]), sort_asc);
  
//   printf("Index: ");
//   printf("%i \n", index);
//   for (int x = 0; x < index * 2; x++){
//     printf("%i ",events[x]);
//   }
//   printf("\n");




//   uint jndex = 0;
//   for(int i =0;i <(index * 2);i++){
//     if (events[i]-prev != 0){
//       if (events[i]-prev > 127){
//         score[chan][jndex++] = 0x7F;
//         score[chan][jndex++] = (byte)(events[i] - prev - 127);
//         printf("FOO ");
//       }else {
//         printf("BAR ");
//         score[chan][jndex++] = (byte)(events[i]-prev);
//       }
//     }
//     for (int x = 0; x < index; x++){
//       if (events[i] == starts[x]){
//         score[chan][jndex++] = 0x90;
//         score[chan][jndex++] = 0xAA;
//         score[chan][jndex++] = 0x41;
//         // prev = starts[x];
//       } else {
//         score[chan][jndex++] = 0x80;
//         score[chan][jndex++] = 0xAA;
//         // prev = ends[x];
//       }
//     }
//     jndex++;
//     printf("%i ", events[i] - prev);
//     prev = events[i];
//   }
//   printf("\n");

//   //score[chan][jndex++] = 512-prev;
//   if (512 - prev > 127){
//     score[chan][jndex++] = 0x7F;
//     score[chan][jndex++] = (byte)((preset.tracks[chan].limit * 32) - prev - 127);
//   } else {
//     score[chan][jndex++] = (byte)((preset.tracks[chan].limit * 32) - prev);
//   }
//   score[chan][jndex++] = 0xE0;
//   if(chan==0){
//     for(int i =0;i<32;i++){
//       printf("0x%02x ",score[chan][i]);
//     }
//     printf("\n");
//   }
// }

void clock_task(void *pvParameters) {
  gpio_init(13);
  gpio_set_dir(13, GPIO_OUT);
  //tbd groove
  int step = 0;
  while (true) {
    xQueueSend(xClock, &step, 0U);
    if (step%16==0){
      //seq_calc(0);
      gpio_put(13, 1);
    }
    else{
      gpio_put(13, 0);
    }
    step++;
    if (step >= 256) step=0;
    vTaskDelay(pdMS_TO_TICKS((60.0/preset.bpm)*60.0));
  }
}

void seq_task(void *pvParameters){
    uint step;
    uint score_cursor[8] = {0,0,0,0,0,0,0,0};
    uint wait_ticks[8] = {1,1,1,1,1,1,1,1};
    byte cmd, opcode, chan, note;
    for(int i =0;i<8;i++){
      seq_calc(i); //recalc all
    }
    while(true){
        xQueueReceive(xClock, &step, portMAX_DELAY);
        for(int i =0;i<8;i++){ //channels
          if (wait_ticks[i] && --wait_ticks[i] == 0) {
            while (true) {
                cmd = score[i][score_cursor[i]++];
                if (cmd < 0x80) { /* wait count in msec. */
                  wait_ticks[i] = (unsigned) cmd;
                  if (wait_ticks[i] == 0) wait_ticks[i] = 1;
                  break;
                }
                opcode = cmd & 0xf0;
                chan = cmd & 0x0f;
                if (opcode == CMD_STOPNOTE) { /* stop note */
                  note = score[i][score_cursor[i]++];
                  // score_cursor[i]++; // ignore it
                  // printf("note off %i channel %i\n",note, chan);
                  ccolors[channel_led[i]] = 0;
                }
                else if (opcode == CMD_PLAYNOTE) { /* play note */
                  note = score[i][score_cursor[i]++];
                  ++score_cursor[i]; // ignore volume
                  //printf("note on %i channel %i\n",note, chan);
                  ccolors[channel_led[i]] = 0x0c0c0c;
                }
                else if (opcode == CMD_INSTRUMENT) { /* change a channel's instrument */
                  score_cursor[i]++; // ignore it
                }
                else if (opcode == CMD_RESTART) { /* restart score */
                  score_cursor[i] = 0;
                  seq_calc(i); //recalc one
                }
              }
            }
            if (step%16==0){ //make gui spinnin'
              playing_step[i]++;
              if (playing_step[i] >= preset.tracks[i].limit) {
                  playing_step[i] = 0;
              }
            }
          }
          rgb_update();
      }
}

int main() {
    preset.bpm = 128;
    stdio_init_all();
    stdio_uart_init_full(uart1, 115200, 20, 21); //debug via stemma
    rgb_init();
    xClock = xQueueCreate(1, sizeof(uint));
    xNote = xQueueCreate(1, sizeof(uint[16]));
    xTaskCreate(clock_task, "clock", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(seq_task, "sequencer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(display_task, "display", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}
