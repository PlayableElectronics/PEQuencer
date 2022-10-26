#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 12
#define WS2812_PIN 19

uint32_t ccolors[NUM_PIXELS] = {0,0,0,0,0,0,0,0,0,0,0,0};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

void rgb_init() {
  // todo get free sm
  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
}

void rgb_update() {
    for (int i = 0; i < NUM_PIXELS; ++i)
      put_pixel(ccolors[i]);
}
