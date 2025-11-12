#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_attr.h"
#include <FastLED.h>

#define INTERRUPT_PIN  GPIO_NUM_17     // Eingangspin für Interrupt

#define NUM_LEDS_PER_STRIP 120

// Pins für die 11 Streifen
#define STRIPE1 GPIO_NUM_0
#define STRIPE2 GPIO_NUM_1
#define STRIPE3 GPIO_NUM_2
#define STRIPE4 GPIO_NUM_3
#define STRIPE5 GPIO_NUM_15
#define STRIPE6 GPIO_NUM_16
#define STRIPE7 GPIO_NUM_18
#define STRIPE8 GPIO_NUM_48
#define STRIPE9 GPIO_NUM_45
#define STRIPE10 GPIO_NUM_46
#define STRIPE11 GPIO_NUM_47

static const char *TAG = "ISR_QUEUE";

static QueueHandle_t gpio_evt_queue = NULL;
hw_timer_t * myTimer = NULL;

// CRGB Arrays für jeden Streifen
CRGB leds1[NUM_LEDS_PER_STRIP];
CRGB leds2[NUM_LEDS_PER_STRIP];
CRGB leds3[NUM_LEDS_PER_STRIP];
CRGB leds4[NUM_LEDS_PER_STRIP];
CRGB leds5[NUM_LEDS_PER_STRIP];
CRGB leds6[NUM_LEDS_PER_STRIP];
CRGB leds7[NUM_LEDS_PER_STRIP];
CRGB leds8[NUM_LEDS_PER_STRIP];
CRGB leds9[NUM_LEDS_PER_STRIP];
CRGB leds10[NUM_LEDS_PER_STRIP];
CRGB leds11[NUM_LEDS_PER_STRIP];

void IRAM_ATTR gpio_isr_handler(void *arg) {
    if(xQueueSendFromISR(gpio_evt_queue, arg, NULL)) {
      // Event queued
    }
}

void setup() {
    Serial.begin(115200);
    
    // FastLED Initialisierung: 11 Streifen, jeder an eigenem Pin
    FastLED.addLeds<WS2812, STRIPE1, GRB>(leds1, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, STRIPE2, GRB>(leds2, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, STRIPE3, GRB>(leds3, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, STRIPE4, GRB>(leds4, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, STRIPE5, GRB>(leds5, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, STRIPE6, GRB>(leds6, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, STRIPE7, GRB>(leds7, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, STRIPE8, GRB>(leds8, NUM_LEDS_PER_STRIP);
    //FastLED.addLeds<WS2812, STRIPE9, GRB>(leds9, NUM_LEDS_PER_STRIP);
    //FastLED.addLeds<WS2812, STRIPE10, GRB>(leds10, NUM_LEDS_PER_STRIP);
    //FastLED.addLeds<WS2812, STRIPE11, GRB>(leds11, NUM_LEDS_PER_STRIP);

    FastLED.setBrightness(128);

    // GPIO-Konfiguration für Interruptpin
    gpio_config_t io_conf_in = {
        .pin_bit_mask = (1ULL << INTERRUPT_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };
    gpio_config(&io_conf_in);

    // Event-Queue initialisieren
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    myTimer = timerBegin(40000000);
    timerStart(myTimer);
}

void setPixel(CRGB *leds, int pixelIndex, uint8_t red, uint8_t green, uint8_t blue){
    leds[pixelIndex] = CRGB(red, green, blue);
}

void sequence(){
    // Fading hoch/runter alle Streifen gleichzeitig
    for(int i = 0; i < 255; i++){
        for(int j = 0; j < NUM_LEDS_PER_STRIP; j++){
            leds1[j] = CRGB(i, i, i);
            leds2[j] = CRGB(i, i, i);
            leds3[j] = CRGB(i, i, i);
            leds4[j] = CRGB(i, i, i);
            leds5[j] = CRGB(i, i, i);
            leds6[j] = CRGB(i, i, i);
            leds7[j] = CRGB(i, i, i);
            leds8[j] = CRGB(i, i, i);
            //leds9[j] = CRGB(i, i, i);
            //leds10[j] = CRGB(i, i, i);
            //leds11[j] = CRGB(i, i, i);
        }
        FastLED.show();
        delay(10);
    }
    for(int i = 255; i > 0; i--){
        for(int j = 0; j < NUM_LEDS_PER_STRIP; j++){
            leds1[j] = CRGB(i, i, i);
            leds2[j] = CRGB(i, i, i);
            leds3[j] = CRGB(i, i, i);
            leds4[j] = CRGB(i, i, i);
            leds5[j] = CRGB(i, i, i);
            leds6[j] = CRGB(i, i, i);
            leds7[j] = CRGB(i, i, i);
            leds8[j] = CRGB(i, i, i);
            //leds9[j] = CRGB(i, i, i);
            //leds10[j] = CRGB(i, i, i);
            //leds11[j] = CRGB(i, i, i);
        }
        FastLED.show();
        delay(10);
    }
}

void loop(){
    sequence();
}
