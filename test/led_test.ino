#include <Adafruit_NeoPixel.h>

#define NUMPIXELS        1
#define PIN_NEOPIXEL 20
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);


void setup() {

#if defined(NEOPIXEL_POWER)
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

  pixels.begin();
  pixels.setBrightness(20);
}

void loop() {
  pixels.fill(0xFF0000);
  pixels.show();
  delay(300);

  pixels.fill(0x00FF00);
  pixels.show();
  delay(300);
  
  pixels.fill(0x0000FF);
  pixels.show();
  delay(300);

  pixels.fill(0xFFFFFF);
  pixels.show();
  delay(300);
}