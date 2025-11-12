#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "driver/gpio.h"
#define HEIGHT 60
#define DISPLAY[11][11][60]
#define INTERRUPT_PIN_IN 0
#define INTERRUPT_PIN_OUT 1 

/*void uint32_t[][][] setUpStripes(){
  
}*/