#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <RotaryEncoder.h>
//#include <Adafruit_TinyUSB.h>
//#include <MIDI.h>
#include <EasyButton.h>
//?? #include "pgmspace.h"

#define NUM_NEOPIXEL  12
#define PIN_NEOPIXEL  19
#define OLED_MOSI     27
#define OLED_CLK      26
#define OLED_DC       24
#define OLED_CS       22
#define OLED_RST      23
#define PIN_SWITCH    0
#define PIN_ROTA      18
#define PIN_ROTB      17

const int LED[8] = {1, 2, 4, 5, 7, 8, 10, 11};
const int MENU_LED[4] = {3, 6, 9, 12};
const byte x16[16] = {74, 84, 91, 96, 98, 96, 91, 84, 74, 64, 57, 52, 50, 52, 57, 64}; //Vertex coordinates
const byte y16[16] = {8,  10,  15,  22,  32, 42, 49, 54, 56, 54, 49, 42, 32, 22, 15,  10}; //Vertex coordinates
char const *noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
const static byte euc16[17][16] PROGMEM = {
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
 {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
 {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
 {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
 {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0},
 {1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0},
 {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
 {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
 {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1},
 {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1},
 {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
 {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1},
 {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
 {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

char const *menus[4][5] = {
  {"STEP", "OFF", "LIMIT", "NOTE", "BPM"},
  {"CHAN", "GATE", "VELO", "EACH", "SWING"},
  {"SIGN", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
  {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"}
};

// Print usefull informations
/*
void PrintInformations(){
  printf("STEP");
  printf("\t");

  printf("OFFSET");
  printf("\t");

  printf("LIMIT");
  printf("\t");

  printf("DEFAULT NOTE");
  printf("\t");

  printf("MUTE");
  printf("\t");

  printf("MIDI CHANNEL");
  printf("\t");

  printf("VELOCITY");
  printf("\t");

  printf("CHANNEL");
  printf("\t");

  printfln();

    for (int x = 0; x < 8; x++) {
      printf(preset[0][x]);
      printf("\t");
      printf(preset[1][x]);
      printf("\t");
      printf(preset[2][x]);
      printf("\t");
      printf(preset[3][x]);
      printf("\t\t");
      printf(preset[4][x]);
      printf("\t");
      printf(preset[6][x]);
      printf("\t\t");
      printf(preset[7][x]);
      printf("\t\t");
      printf(x+1);
      printf("\t");
      printfln();
    }
  printfln();
}
*/
