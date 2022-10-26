#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 12
#define WS2812_PIN 19

uint32_t ccolors[NUM_PIXELS] = {0,0xF,0,0,0,0,0,0,0,0,0xF,0};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

void set_pixel(uint pixel, uint32_t pixel_grb) {
    ccolors[pixel] = pixel_grb;
    for (int i = 0; i < NUM_PIXELS; ++i)
      if(i==pixel)
        put_pixel(pixel_grb);
      else
        put_pixel(ccolors[i]);
}

void rgb_init() {
  // todo get free sm
  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);

  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

}
/*
void channel_task(void *channel) {
  uint *chan;
  chan = (uint *) channel;
  while (true) {
    //for (int i = 0; i < NUM_PIXELS; ++i){
      set_pixel(channel, 0xFFFFFF);
      vTaskDelay(10);
      set_pixel(channel, 0);
      vTaskDelay(30);
    //}
  }
}

void test_task() {
  while (true) {
    set_pixel(11, 0xFFFFFF);
    vTaskDelay(10);
    set_pixel(11, 0);
    vTaskDelay(29);
  }
}
*/
