#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "rgb.h"
//#include "Playtune.h"
#include "ui/ui.h"
//#include <diatonic.h>

//const byte PROGMEM score [] = {
/*const byte score [] = {
  0x90,0xAA,0x41,0x00,0x04,
  0x80,0x00,0x02,
  0x91,0xAA,0x41,0x00,0x04,
  0x81,0x00,0x02,
  0x90,0xAA,0x41,0x91,0xAA,0x41,0x00,0x04,
  0x80,0x81,0x00,0x02,
  0xE0};*/

  byte score[8][128] = {
      {0xe0},
      //{0x50, 0x90, 0xaa, 0x41, 0x01, 0x80, 0xaa, 0x7f, 0x01, 0x90, 0xaa, 0x41, 0x01, 0x80, 0xaa, 0x7f, 0x01, 0x90, 0xaa, 0x41, 0x01, 0x80, 0xaa, 0x7f, 0x01, 0x90, 0xaa, 0x41, 0x01, 0x80, 0xaa, 0x20, 0xe0},
      //{0x30, 0x90, 0xaa, 0x41, 0x01, 0x80, 0xaa, 0x7f, 0x21, 0x90, 0xaa, 0x41, 0x01, 0x80, 0xaa, 0x40, 0xe0},
      //{0x90,0xAA,0x41,0x00,0x09,0x80,0xAA,0x00,0x02,0xE0},
      //{0x90,0xAA,0x41,0x09,0x80,0xAA,0x02,0xE0},
      {0xe0},
      {0xe0},
      {0xe0},
      {0xe0},
      {0xe0},
      {0xe0},
      {0xe0}
  };

#define CMD_PLAYNOTE	0x90	/* play a note: low nibble is generator #, note is next byte */
#define CMD_STOPNOTE	0x80	/* stop a note: low nibble is generator # */
#define CMD_INSTRUMENT  0xc0 /* change instrument; low nibble is generator #, instrument is next byte */
#define CMD_RESTART	0xe0	/* restart the score from the beginning */
#define CMD_STOP	0xf0	/* stop playing */
/* if CMD < 0x80, then the other 7 bits and the next byte are a 15-bit big-endian number of msec to wait */

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
