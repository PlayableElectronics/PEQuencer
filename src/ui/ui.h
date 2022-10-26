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
  uint bpm;
};

struct Preset {
  struct Track tracks[16];
};

struct Track track;
struct Preset preset;

int eachNote[8][16] = {
 {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
 {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
 {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
 {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
 {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
 {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
 {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
 {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48}
};

unsigned long gate_timer = 0; //countermeasure of sw chattering

// Sequence variable
byte buf_count = 0;
byte line_xbuf[17];//Buffer for drawing lines
byte line_ybuf[17];//Buffer for drawing lines

// Global variables store
byte playing_step[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //playing step number , CH1,2,3,4,5,6

// Menu Positions
int j = 0;
int menuPosition = 1;
int subMenuPosition = 1;
int newPos = 0;

int subMenu1Pos = 1;
int subMenu2Pos = 1;
int subMenu3Pos = 1;
int subMenu4Pos = 1;

// debounce
int debounce = 5;

// int preset[9][8] ={
//   {4, 4, 5, 3, 2, 16, 1, 3},                        // each channel step
//   {0, 2, 0, 8, 3, 9, 2 ,2},                         // each channel offset
//   {16, 16, 16, 16, 16, 16, 16, 16},                 // each channel limit
//   {48, 48, 48, 48, 48, 48, 48, 48},                 // each channel default note
//   {1, 0, 0, 0, 0, 0, 0, 0},                         // each channel mute 0 = off , 1 = on
//   {10, 10, 10, 10, 10, 10, 10, 10},                 // each channel gate timer
//   {1, 2, 3, 4, 5, 6, 7, 8},                         // midi channel
//   {127, 127, 127, 127, 127, 127, 127, 127},         // each channel velocity
//   {132, 132, 132, 132, 132, 132, 132, 132}          // BPM
// };

bool offset_buf[8][16];//offset buffer , Stores the offset result
int select_ch = 1; //0~5 = each channel -1 , 6 = random mode
float BPM = 132.0;

// Timer
int t1 = 0;
int t2 = 0;

EasyButton button1(LED[0], debounce, true, false);
EasyButton button2(LED[1], debounce, true, false);
EasyButton button3(LED[2], debounce, true, false);
EasyButton button4(LED[3], debounce, true, false);
EasyButton button5(LED[4], debounce, true, false);
EasyButton button6(LED[5], debounce, true, false);
EasyButton button7(LED[6], debounce, true, false);
EasyButton button8(LED[7], debounce, true, false);
EasyButton menu_button1(MENU_LED[0], debounce, true, false);
EasyButton menu_button2(MENU_LED[1], debounce, true, false);
EasyButton menu_button3(MENU_LED[2], debounce, true, false);
EasyButton menu_button4(MENU_LED[3], debounce, true, false);
EasyButton encoderButton(PIN_SWITCH, debounce, true, false);

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);
//RotaryEncoder encoder2(21, 20, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder encoder(PIN_ROTB, PIN_ROTA, RotaryEncoder::LatchMode::FOUR3);
//Adafruit_USBD_MIDI usb_midi;
//MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, usbMIDI);

// Fix Encoder setPosition
void fixEncoderMenu(int channel){
  switch (menuPosition){
      case 1:
        switch (subMenuPosition){
          case 1:
            encoder.setPosition(preset.tracks[channel -1].step);
            break;
          case 2:
            encoder.setPosition(preset.tracks[channel -1].offset);
            break;
          case 3:
            encoder.setPosition(preset.tracks[channel -1].limit);
            break;
          case 4:
            encoder.setPosition(preset.tracks[channel -1].note);
            break;
          case 5:
            encoder.setPosition(preset.tracks[channel -1].bpm);
            break;
        }
        break;
      case 2:
        switch (subMenuPosition){
          case 1:
            encoder.setPosition(preset.tracks[channel -1].channel);
            break;
          case 2:
            encoder.setPosition(preset.tracks[channel -1].gate);
            break;
          case 3:
            encoder.setPosition(preset.tracks[channel -1].velocity);
            break;
          case 4:
            break;
          case 5:
            break;
        }
        break;
      case 3:
        switch (subMenuPosition){
          case 1:
            break;
          case 2:
            break;
          case 3:
            break;
          case 4:
            break;
          case 5:
            break;
        }
        break;
      case 4:
        switch (subMenuPosition){
          case 1:
            break;
          case 2:
            break;
          case 3:
            break;
          case 4:
            break;
          case 5:
            break;
        }
        break;
    }
}

// Get MidiNotes from Integers
String midiNotes(int noteNumber){
    byte octave = noteNumber / 12;
    byte noteInOctave = noteNumber % 12;
    return *noteNames[noteInOctave] + String(octave - 1);
}

// Get subMenu position
int encoderClickMenu(int LIMIT, int MENU){
  if (subMenuPosition > LIMIT){
    subMenuPosition = 1;
    fixEncoderMenu(select_ch);
  }
  switch (MENU) {
    case 1:
      subMenu1Pos = subMenuPosition;
      break;
    case 2:
      subMenu2Pos = subMenuPosition;
      break;
    case 3:
      subMenu3Pos = subMenuPosition;
      break;
    case 4:
      subMenu4Pos = subMenuPosition;
      break;
  }
  return subMenuPosition;
}

// Callback for encoder click
void encoderClick(){
  subMenuPosition = subMenuPosition + 1;
  fixEncoderMenu(select_ch);
}

// Get encoder Position, set Min and Max value
int getEncoderPosition(int ROTARYMIN, int ROTARYMAX, bool JUMP_TO){
    newPos = encoder.getPosition();
    if (newPos > ROTARYMAX){
        if(JUMP_TO == false){
          newPos = ROTARYMAX;
          encoder.setPosition(ROTARYMAX);
        } else if (JUMP_TO == true){
          newPos = ROTARYMIN;
          encoder.setPosition(ROTARYMIN);
        }

    } else if (newPos < ROTARYMIN){
        if(JUMP_TO == false){
          newPos = ROTARYMIN;
          encoder.setPosition(ROTARYMIN);
        }else if(JUMP_TO == true){
          newPos = ROTARYMAX;
          encoder.setPosition(ROTARYMAX);
        }
    
    }
    return newPos;
}

// Draw statics lines and dots
void drawStatics(int current_page){
  
  display.setTextColor(SH110X_WHITE);
  for (int m = 0; m < 5; m++) {
    display.setCursor(0, m*10);
    display.print(menus[current_page][m]);
  }
  
  display.fillRect(0, 53, 9, 12, SH110X_WHITE);
  if (preset.tracks[select_ch -1].mute == 0){
      display.fillRect(0, 53, 40, 12, SH110X_WHITE);
      display.setTextColor(SH110X_BLACK);
      display.setCursor(2, 55);
      display.print(select_ch);
      display.setCursor(9, 55);
      display.print("MUTED");
  } else {
      display.fillRect(0, 53, 9, 12, SH110X_WHITE);
      display.setTextColor(SH110X_BLACK);
      display.setCursor(2, 55);
      display.print(select_ch);
  }
  display.setTextColor(SH110X_WHITE);
  for(int i = 0; i < 4; i++){
    //pixels.setPixelColor(MENU_LED[i]-1, 0x001F00);
  }
  //pixels.setPixelColor(MENU_LED[current_page]-1, 0x00FF00);
  if (current_page == 0){
    
    for (int x = 0; x < 8; x++){
    if (x == select_ch -1){
      buf_count = 0;
      for (int m = 0; m < 16; m++) {
        if (offset_buf[select_ch - 1][m] == 1) {
          line_xbuf[buf_count] = x16[m];
          line_ybuf[buf_count] = y16[m];
          buf_count++;
        }
      }
    }
  }

  for (int k = 0; k < 8; k++){
    for (int i = preset.tracks[k].offset; i < 16; i++) {
      offset_buf[k][i - preset.tracks[k].offset] = (pgm_read_byte(&(euc16[preset.tracks[k].step][i]))) ;
    }
    for (int i = 0; i < preset.tracks[k].offset; i++) {
      offset_buf[k][16 - preset.tracks[k].offset + i] = (pgm_read_byte(&(euc16[preset.tracks[k].step][i])));
    }
  }


    for (int j = 0; j <= preset.tracks[select_ch -1].limit; j++) { // j = steps
      display.drawPixel(x16[j], y16[j], SH110X_WHITE);
    }
    if (preset.tracks[select_ch -1].step > 1){
      for (j = 0; j < buf_count - 1; j++) {
        display.drawLine(line_xbuf[j], line_ybuf[j], line_xbuf[j + 1], line_ybuf[j + 1], SH110X_WHITE);
      }
      display.drawLine(line_xbuf[0], line_ybuf[0], line_xbuf[j], line_ybuf[j], SH110X_WHITE);
  }
  }
}

// Fix Encoder click for each MENU
void fixEncoderClick(int Menu){
  switch (Menu) {
    case 1:
      subMenuPosition = subMenu1Pos;
      break;
    case 2:
      subMenuPosition = subMenu2Pos;
      break;
    case 3:
      subMenuPosition = subMenu3Pos;
      break;
    case 4:
      subMenuPosition = subMenu4Pos;
      break;
  }
}

// Select and mute channel
void selectChannel(int channel){
  if (channel == select_ch){
    preset.tracks[select_ch -1].mute = !preset.tracks[select_ch -1].mute;
  } else{
    fixEncoderMenu(channel);
  }
  select_ch = channel;
}

// Events Read
void eventsRead(){
  menu_button1.read();
  menu_button2.read();
  menu_button3.read();
  menu_button4.read();
  button1.read();
  button2.read();
  button3.read();
  button4.read();
  button5.read();
  button6.read();
  button7.read();
  button8.read();
  encoderButton.read();
}

// Fill black rect and draw fixed text over this rect

void Step(){
  display.fillRect(68, 27, 13,9, SH110X_BLACK );
  preset.tracks[select_ch -1].step = getEncoderPosition(0, 16, true);
  if (preset.tracks[select_ch -1].step >= 10){
      display.fillRect(68, 27, 13,9, SH110X_BLACK );
      display.setCursor(69, 28);
      display.print(preset.tracks[select_ch -1].step);
  } else {
      display.fillRect(71, 27, 7,9, SH110X_BLACK );
      display.setCursor(72, 28);
      display.print(preset.tracks[select_ch -1].step);
  }
}

void Offset(){
  display.fillRect(68, 27, 13,9, SH110X_BLACK );
  preset.tracks[select_ch -1].offset = getEncoderPosition(0, 15, true);
  if (preset.tracks[select_ch -1].offset >= 10){
      display.fillRect(68, 27, 13,9, SH110X_BLACK );
      display.setCursor(69, 28);
      display.print(preset.tracks[select_ch -1].offset);
  } else {
      display.fillRect(71, 27, 7,9, SH110X_BLACK );
      display.setCursor(72, 28);
      display.print(preset.tracks[select_ch -1].offset);
  }
}

void Limit(){
  display.fillRect(68, 27, 13,9, SH110X_BLACK );
  preset.tracks[select_ch -1].limit = getEncoderPosition(0, 16, true);
  if (preset.tracks[select_ch -1].limit >= 10){
      display.fillRect(68, 27, 13,9, SH110X_BLACK );
      display.setCursor(69, 28);
      display.print(preset.tracks[select_ch -1].limit);
  } else {
      display.fillRect(71, 27, 7,9, SH110X_BLACK );
      display.setCursor(72, 28);
      display.print(preset.tracks[select_ch -1].limit);
  }
}

void Note(){
  display.fillRect(68, 27, 13,9, SH110X_BLACK );
  preset.tracks[select_ch -1].note = getEncoderPosition(21, 108, true);
  if (preset.tracks[select_ch -1].note >= 10){
      display.fillRect(68, 27, 13,9, SH110X_BLACK );
      display.setCursor(69, 28);
      display.print(midiNotes(preset.tracks[select_ch -1].note));
  } else {
      display.fillRect(71, 27, 7,9, SH110X_BLACK );
      display.setCursor(72, 28);
      display.print(midiNotes(preset.tracks[select_ch -1].note));
  }
}

void Bpm(){
  display.fillRect(68, 27, 13,9, SH110X_BLACK );
  preset.tracks[select_ch -1].bpm = getEncoderPosition(0, 255, true);
  if (preset.tracks[select_ch -1].bpm >= 10){
      display.fillRect(68, 27, 13,9, SH110X_BLACK );
      display.setCursor(69, 28);
      display.print(preset.tracks[select_ch -1].bpm);
  } else {
      display.fillRect(71, 27, 7,9, SH110X_BLACK );
      display.setCursor(72, 28);
      display.print(preset.tracks[select_ch -1].bpm);
  }
}

// ***** ***** ***** *****
//
//  FEATURES FROM MENU 2
//
// ***** ***** ***** *****

void ChannelAssignment(){
   preset.tracks[select_ch -1].channel = getEncoderPosition(1, 8, true);
   int cursorX = 40;
   int cursorY = 15;

   for (int cursorpos = 1; cursorpos < 9; cursorpos++) {
        display.setCursor(cursorX, cursorY);
        if (preset.tracks[select_ch -1].mute == 0){
            display.drawRect(cursorX-4, cursorY-4, 13,15, SH110X_WHITE);
        }
        if (preset.tracks[select_ch -1].channel == cursorpos){
            display.setTextColor(SH110X_BLACK);
            display.fillRect(cursorX-4, cursorY-4, 13,15, SH110X_WHITE);
            display.print(cursorpos);
        } else {
            //pixels.setPixelColor(2, 0x000000);
            display.setTextColor(SH110X_WHITE);
            display.print(cursorpos);
        }
        if (cursorpos == 4){
            cursorX = 17;
            cursorY += 20;
        }
        cursorX += 23;
    }
}

void GateTimer(){
  display.fillRect(68, 27, 13,9, SH110X_BLACK );
  preset.tracks[select_ch -1].gate = getEncoderPosition(0, 200, true);
  display.setTextSize(2);
  display.setCursor(40, 22);
  display.print(preset.tracks[select_ch -1].gate);
  display.println("/200");
  display.setTextSize(1);
}

void Velocity(){
  preset.tracks[select_ch -1].velocity = getEncoderPosition(0, 127, true);
  display.drawLine(45, 32, 45, 40, SH110X_WHITE);
  display.drawLine(111, 32, 111, 40, SH110X_WHITE);
  display.fillRect(47, 35, int(preset.tracks[select_ch -1].velocity/2), 3, SH110X_WHITE);
  display.setTextSize(2);
  if (preset.tracks[select_ch -1].velocity < 10){
      display.setCursor(76, 5);
  } else if (preset.tracks[select_ch -1].velocity < 100){
      display.setCursor(68, 5);
  } else if (preset.tracks[select_ch -1].velocity < 1000){
      display.setCursor(63, 5);
  }
  display.print(preset.tracks[select_ch -1].velocity);
  display.setTextSize(1);
}

// qsort requires you to create a sort function
int sort_desc(const void *cmp1, const void *cmp2)
{
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
  return a > b ? -1 : (a < b ? 1 : 0);
  // A simpler, probably faster way:
  //return b - a;
}

void EachStepNote(){
  int index_list = 0;
  int len = preset.tracks[select_ch -1].step;
  int temp_list[len] = {};
  int offset_temp_x = 0;

  for (int temp_x = 0; temp_x < 16; temp_x ++) {
    if (euc16[preset.tracks[select_ch -1].step][temp_x] == 1){
        offset_temp_x = temp_x + preset.tracks[select_ch -1].offset;
        while (offset_temp_x > 16){
           offset_temp_x = offset_temp_x - 16;
        }
        temp_list[index_list] = offset_temp_x;
        index_list++;
    }
  }
  int lt_length = sizeof(temp_list) / sizeof(temp_list[0]);
  qsort(temp_list, lt_length, sizeof(temp_list[0]), sort_desc);

  int cursorX = 37;
  int cursorY = 2;

  for(int x = 0; x < len; x++){
    display.setCursor(cursorX, cursorY);
    display.fillRect(cursorX-1, cursorY-1, 13,9, SH110X_WHITE);
    cursorX += 21;
    
    if (x == 3 || x == 7 || x == 11){
      cursorX = 37;
      cursorY += 18;
    }

  }
  cursorX = 0;
  cursorY = 0;

}

void menu1(int subPosition){
  switch (subPosition){
    case 1: //STEP
      display.fillTriangle(26, 3, 30, 0, 30, 6, SH110X_WHITE);
      Step();
      break;
    case 2: //OFFSET
      display.fillTriangle(37, 13, 42, 10, 42, 16, SH110X_WHITE);
      Offset();
      break;
    case 3: //LIMIT
      display.fillTriangle(32, 23, 36, 20, 36, 26, SH110X_WHITE);
      Limit();
      break;
    case 4: //NOTE
      display.fillTriangle(26, 33, 30, 30, 30, 36, SH110X_WHITE);
      Note();
      break;
    case 5: //BPM
      display.fillTriangle(19, 43, 23, 40, 23, 46, SH110X_WHITE);
      Bpm();
      break;
  }
}

void menu2(int subPosition){
  switch (subPosition){
    case 1: //CHANNEL
      display.fillTriangle(27, 3, 31, 0, 31, 6, SH110X_WHITE);
      ChannelAssignment();
      display.fillTriangle(27, 3, 31, 0, 31, 6, SH110X_WHITE);
      break;
    case 2: //GATE
      display.fillTriangle(27, 13, 31, 10, 31, 16, SH110X_WHITE);
      GateTimer();
      break;
    case 3: //VELOCITY
      display.fillTriangle(27, 23, 31, 20, 31, 26, SH110X_WHITE);
      Velocity();
      break;
    case 4: //EACH
      display.fillTriangle(27, 33, 31, 30, 31, 36, SH110X_WHITE);
      EachStepNote();
      break;
    case 5: //SIGN
      display.fillTriangle(27, 43, 31, 40, 31, 46, SH110X_WHITE);
      break;
  }
}
void menu3(int subPosition){
  switch (subPosition){
    case 1: //SWING
      display.fillTriangle(32, 3, 36, 0, 36, 6, SH110X_WHITE);
      break;
    case 2: //EMPTY
      display.fillTriangle(32, 13, 36, 10, 36, 16, SH110X_WHITE);
      break;
    case 3: //EMPTY
      display.fillTriangle(32, 23, 36, 20, 36, 26, SH110X_WHITE);
      break;
    case 4: //EMPTY
      display.fillTriangle(32, 33, 36, 30, 36, 36, SH110X_WHITE);
      break;
    case 5: //EMPTY
      display.fillTriangle(32, 43, 36, 40, 36, 46, SH110X_WHITE);
      break;
  }
}
void menu4(int subPosition){
  switch (subPosition){
    case 1: //EMPTY
      display.fillTriangle(32, 3, 36, 0, 36, 6, SH110X_WHITE);
      break;
    case 2: //EMPTY
      display.fillTriangle(32, 13, 36, 10, 36, 16, SH110X_WHITE);
      break;
    case 3: //EMPTY
      display.fillTriangle(32, 23, 36, 20, 36, 26, SH110X_WHITE);
      break;
    case 4: //EMPTY
      display.fillTriangle(32, 33, 36, 30, 36, 36, SH110X_WHITE);
      break;
    case 5: //EMPTY
      display.fillTriangle(32, 43, 36, 40, 36, 46, SH110X_WHITE);
      break;
  }
}

void drawMenu(){

  switch(menuPosition){
    case 1:
      drawStatics(0);
      menu1(encoderClickMenu(5, 1));
      
      break;
    case 2:
      drawStatics(1);
      menu2(encoderClickMenu(5, 2));
      break;
    case 3:
      drawStatics(2);
      menu3(encoderClickMenu(5, 3));
      break;
    case 4:
      drawStatics(3);
      menu4(encoderClickMenu(5, 4));
      break;
  }
}

// Encoders
void checkPosition() {  encoder.tick(); }

// Menu Buttons
void MenuPressed1() { menuPosition = 1; fixEncoderClick(1); fixEncoderMenu(select_ch);}
void MenuPressed2() { menuPosition = 2; fixEncoderClick(2); fixEncoderMenu(select_ch);}
void MenuPressed3() { menuPosition = 3; fixEncoderClick(3); fixEncoderMenu(select_ch);}
void MenuPressed4() { menuPosition = 4; fixEncoderClick(4); fixEncoderMenu(select_ch);}

// Channel Buttons
void onPressed1() { selectChannel(1);}
void onPressed2() { selectChannel(2);}
void onPressed3() { selectChannel(3);}
void onPressed4() { selectChannel(4);}
void onPressed5() { selectChannel(5);}
void onPressed6() { selectChannel(6);}
void onPressed7() { selectChannel(7);}
void onPressed8() { selectChannel(8);}

void display_task(void *pvParameters){
    display.begin(0, true);
    display.setTextSize(1);
    display.setTextColor(SH110X_BLACK, SH110X_WHITE);
    display.clearDisplay();
    //display.fillTriangle(27, 3, 31, 0, 31, 6, SH110X_WHITE);
    //display.display();
    printf("foo");
    pinMode(PIN_ROTA, INPUT_PULLUP);
    pinMode(PIN_ROTB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ROTA), checkPosition, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ROTB), checkPosition, CHANGE);

    menu_button1.begin();
    menu_button2.begin();
    menu_button3.begin();
    menu_button4.begin();

    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();
    button5.begin();
    button6.begin();
    button7.begin();
    button8.begin();

    menu_button1.onPressed(MenuPressed1);
    menu_button2.onPressed(MenuPressed2);
    menu_button3.onPressed(MenuPressed3);
    menu_button4.onPressed(MenuPressed4);

    button1.onPressed(onPressed1);
    button2.onPressed(onPressed2);
    button3.onPressed(onPressed3);
    button4.onPressed(onPressed4);
    button5.onPressed(onPressed5);
    button6.onPressed(onPressed6);
    button7.onPressed(onPressed7);
    button8.onPressed(onPressed8);

    encoderButton.begin();
    encoderButton.onPressed(encoderClick);

    while(true){
      display.clearDisplay();
      eventsRead();
      drawMenu();
      
      display.display();
      //vTaskDelay();
    }
}
