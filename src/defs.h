#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "rgb.h"
#include "ui/ui.h"

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
