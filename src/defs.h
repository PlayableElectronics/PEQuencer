#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "rgb.h"
#include "Playtune.h"
#include "ui/ui.h"
#include <diatonic.h>

//const byte PROGMEM score [] = {
const byte score [] = {
  0x90,0xAA,0x41,0x00,0x04,
  0x80,0x00,0x02,
  0x91,0xAA,0x41,0x00,0x04,
  0x81,0x00,0x02,
  0x90,0xAA,0x41,0x91,0xAA,0x41,0x00,0x04,
  0x80,0x81,0x00,0x02,
  0xE0};

struct Note {
  uint state;
  uint channel;
  uint pitch;
  int  shift;
  uint velocity;
  uint length;
};

struct Note note;
static QueueHandle_t xClock, xSeq, xNote  = NULL;
