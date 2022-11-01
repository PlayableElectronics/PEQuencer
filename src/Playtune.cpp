/*********************************************************************************************
  Strongely based on Playtune by Len Shustek
  ------------------------------------------------------------------------------------
   The MIT License (MIT)
   Copyright (c) 2011, 2016, Len Shustek

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
  Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
**************************************************************************************/
/*
  -----------------------------------------------------------------------------------------*/

#include "Playtune.h"

#ifndef DBUG
#define DBUG 0          // debugging?
#endif
#define ASSUME_VOLUME 1 // assume volume information is present in bytestream files without headers?

byte _tune_num_chans = 3;

volatile unsigned wait_timer_frequency2;       /* its current frequency */
volatile unsigned wait_timer_old_frequency2;   /* its previous frequency */
volatile boolean wait_timer_playing = false;   /* is it currently playing a note? */
volatile boolean doing_delay = false;          /* are we using it for a tune_delay()? */
volatile unsigned long wait_toggle_count;      /* countdown score waits */
volatile unsigned long delay_toggle_count;     /* countdown tune_ delay() delays */

volatile const byte *score_start = 0;
volatile const byte *score_cursor = 0;
volatile boolean Playtune::tune_playing = false;
boolean volume_present = ASSUME_VOLUME;

void tune_playnote (byte chan, byte note);
void tune_stopnote (byte chan);
void tune_stepscore (void);

//-----------------------------------------------
// Start playing a note on a particular channel
//-----------------------------------------------

void tune_playnote (byte chan, byte note) {
  printf("note on %i channel %i\n",note, chan);
  ccolors[channel_led[chan]] = 0x0c0c0c;
}

//-----------------------------------------------
// Stop playing a note on a particular channel
//-----------------------------------------------

void tune_stopnote (byte chan) {
  printf("note off channel %i\n",chan);
  ccolors[channel_led[chan]] = 0;
}

//-----------------------------------------------
// Start playing a score
//-----------------------------------------------

void Playtune::tune_setscore (const byte *score) {
  if (tune_playing) tune_stopscore();
  score_start = score_cursor = score;
  tune_stepscore();  /* execute initial commands */
  Playtune::tune_playing = true;  /* release the interrupt routine */
}

void Playtune::tune_clock (void) {
  if (Playtune::tune_playing && wait_toggle_count && --wait_toggle_count == 0) {
    tune_stepscore ();
  }
}

void tune_stepscore (void) {
  byte cmd, opcode, chan, note;
  unsigned duration;
  /* Do score commands until a "wait" is found, or the score is stopped.
    This is called initially from tune_playcore, but then is called
    from the interrupt routine when waits expire.
  */
  #define CMD_PLAYNOTE	0x90	/* play a note: low nibble is generator #, note is next byte */
  #define CMD_STOPNOTE	0x80	/* stop a note: low nibble is generator # */
  #define CMD_INSTRUMENT  0xc0 /* change instrument; low nibble is generator #, instrument is next byte */
  #define CMD_RESTART	0xe0	/* restart the score from the beginning */
  #define CMD_STOP	0xf0	/* stop playing */
  /* if CMD < 0x80, then the other 7 bits and the next byte are a 15-bit big-endian number of msec to wait */

  while (1) {
    cmd = pgm_read_byte(score_cursor++);
    if (cmd < 0x80) { /* wait count in msec. */
      duration = ((unsigned)cmd << 8) | (pgm_read_byte(score_cursor++));
      wait_toggle_count = (unsigned long) duration;
      if (wait_toggle_count == 0) wait_toggle_count = 1;
      break;
    }
    opcode = cmd & 0xf0;
    chan = cmd & 0x0f;
    if (opcode == CMD_STOPNOTE) { /* stop note */
      tune_stopnote (chan);
    }
    else if (opcode == CMD_PLAYNOTE) { /* play note */
      note = pgm_read_byte(score_cursor++); // argument evaluation order is undefined in C!
      if (volume_present) ++score_cursor; // ignore volume if present
      tune_playnote (chan, note);
    }
    else if (opcode == CMD_INSTRUMENT) { /* change a channel's instrument */
      score_cursor++; // ignore it
    }
    else if (opcode == CMD_RESTART) { /* restart score */
      score_cursor = score_start;
    }
    else if (opcode == CMD_STOP) { /* stop score */
      Playtune::tune_playing = false;
      break;
    }
  }
}

//-----------------------------------------------
// Stop playing a score
//-----------------------------------------------

void Playtune::tune_stopscore (void) {
  int i;
  for (i = 0; i < _tune_num_chans; ++i)
    tune_stopnote(i);
  Playtune::tune_playing = false;
}
