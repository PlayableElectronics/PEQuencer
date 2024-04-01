#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "pico/stdlib.h"
#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include <stdlib.h>
#include <task.h>
#include <Adafruit_SH110X.h>
#include "logger.hpp"

namespace consts {
  constexpr int NUM_NEOPIXEL = 12;
  constexpr int PIN_NEOPIXEL = 19;
  constexpr int OLED_MOSI = 27;
  constexpr int OLED_CLK = 26;
  constexpr int OLED_DC = 24;
  constexpr int OLED_CS = 22;
  constexpr int OLED_RST = 23;
  constexpr int PIN_SWITCH = 0;
  constexpr int PIN_ROTA = 18;
  constexpr int PIN_ROTB = 17;
};

#endif
