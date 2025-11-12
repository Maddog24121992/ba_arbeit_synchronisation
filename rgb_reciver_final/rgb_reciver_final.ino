#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_attr.h"
//#include <FastLED_NeoPixel.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define INTERRUPT_PIN GPIO_NUM_17  // Eingangspin für Interrupt
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

#define NUMBPIXELS 120


static const char *TAG = "ISR_QUEUE";

static QueueHandle_t gpio_evt_queue = NULL;
static int pin_level_glob = 0;
hw_timer_t *myTimer = NULL;
Adafruit_NeoPixel pixels1(NUMBPIXELS, STRIPE1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(NUMBPIXELS, STRIPE2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels3(NUMBPIXELS, STRIPE3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels4(NUMBPIXELS, STRIPE4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels5(NUMBPIXELS, STRIPE5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels6(NUMBPIXELS, STRIPE6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels7(NUMBPIXELS, STRIPE7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels8(NUMBPIXELS, STRIPE8, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels9(NUMBPIXELS, STRIPE9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels10(NUMBPIXELS, STRIPE10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels11(NUMBPIXELS, STRIPE11, NEO_GRB + NEO_KHZ800);
/*FastLED_NeoPixel<NUMBPIXELS, STRIPE1, NEO_GRB + NEO_KHZ800> pixels1;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE2, NEO_GRB + NEO_KHZ800> pixels2;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE3, NEO_GRB + NEO_KHZ800> pixels3;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE4, NEO_GRB + NEO_KHZ800> pixels4;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE5, NEO_GRB + NEO_KHZ800> pixels5;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE6, NEO_GRB + NEO_KHZ800> pixels6;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE7, NEO_GRB + NEO_KHZ800> pixels7;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE8, NEO_GRB + NEO_KHZ800> pixels8;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE9, NEO_GRB + NEO_KHZ800> pixels9;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE10, NEO_GRB + NEO_KHZ800> pixels10;      // <- The new FastLED_NeoPixel object
FastLED_NeoPixel<NUMBPIXELS, STRIPE11, NEO_GRB + NEO_KHZ800> pixels11;      // <- The new FastLED_NeoPixel object*/

// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, COLOR_ORDER);  // <- The original Adafruit_NeoPixel object
//FastLED_NeoPixel<LED_COUNT, LED_PIN, COLOR_ORDER> strip;      // <- The new FastLED_NeoPixel object
void setPixel(Adafruit_NeoPixel stripe, int pixelIndex, int red, int blue, int green) {
  //int pixelCorrespondIndex = 119-pixelIndex;
  stripe.setPixelColor(pixelIndex, stripe.Color(red, blue, green));
  //stripe.setPixelColor(pixelCorrespondIndex, stripe.Color(red, blue, green));
  stripe.show();
}

// ISR – wird bei steigender Flanke auf IN_GPIO ausgelöst
void IRAM_ATTR gpio_isr_handler(void *arg) {
  //ESP_LOGI(TAG,"ISR ausgelöst! GPIO: "); // Debug-Log für Interrupt
  if (xQueueSendFromISR(gpio_evt_queue, arg, NULL)) {

    //ESP_LOGI(TAG, "Event in Queue");  // → Event in Queue
  } else {
    //ESP_LOGI(TAG, "Queue full, event not sent");  // → Queue voll
  }
}
void loop() {
  sequence();
}



void setup() {

  /*gpio_config_t io_conf1 = {
        .pin_bit_mask = 1ULL << STRIPE1, // Bitmask for GPIO 0
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf2 = {
        .pin_bit_mask = 1ULL << STRIPE2, // Bitmask for GPIO 1
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf3 = {
        .pin_bit_mask = 1ULL << STRIPE3, // Bitmask for GPIO 2
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf4 = {
        .pin_bit_mask = 1ULL << STRIPE4, // Bitmask for GPIO 3
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf5 = {
        .pin_bit_mask = 1ULL << STRIPE5, // Bitmask for GPIO 15
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf6 = {
        .pin_bit_mask = 1ULL << STRIPE6, // Bitmask for GPIO 16
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf7 = {
        .pin_bit_mask = 1ULL << STRIPE7, // Bitmask for GPIO 17
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf8 = {
        .pin_bit_mask = 1ULL << STRIPE8, // Bitmask for GPIO 18
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf9 = {
        .pin_bit_mask = 1ULL << STRIPE9, // Bitmask for GPIO 21
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf10 = {
        .pin_bit_mask = 1ULL << STRIPE10, // Bitmask for GPIO 45
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf11 = {
        .pin_bit_mask = 1ULL << STRIPE11, // Bitmask for GPIO 46
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf1);
    gpio_config(&io_conf2);
    gpio_config(&io_conf3);
    gpio_config(&io_conf4);
    gpio_config(&io_conf5);
    gpio_config(&io_conf6);
    gpio_config(&io_conf7);
    gpio_config(&io_conf8);
    gpio_config(&io_conf9);
    gpio_config(&io_conf10);
    gpio_config(&io_conf11);
    */
  // Konfiguriere IN_GPIO (Eingang, Interrupt auf beliebige Flanke)
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
  //xTaskCreate(gpio_event_task, "gpio_event_task", 4096, NULL, 10, NULL);
  pixels1.begin();
  pixels2.begin();
  pixels3.begin();
  pixels4.begin();
  pixels5.begin();
  pixels6.begin();
  pixels7.begin();
  pixels8.begin();
  pixels9.begin();
  pixels10.begin();
  pixels11.begin();
}

void sequence() {
  //for(int i = 0; i < 255; i++){
  int i = 50;
  int k = 50;
  for (int j = 0; j < 120; j++) {
    pixels1.setPixelColor(j, pixels1.Color(i, i, i));
    pixels2.setPixelColor(j, pixels2.Color(i, i, i));
    pixels3.setPixelColor(j, pixels3.Color(i, i, i));
    pixels4.setPixelColor(j, pixels4.Color(i, i, i));
    pixels5.setPixelColor(j, pixels5.Color(i, i, i));
    pixels6.setPixelColor(j, pixels6.Color(i, i, i));
    pixels7.setPixelColor(j, pixels7.Color(i, i, i));
    pixels8.setPixelColor(j, pixels8.Color(i, i, i));
    pixels9.setPixelColor(j, pixels9.Color(i, i, i));
    pixels10.setPixelColor(j, pixels10.Color(i, i, i));
    pixels11.setPixelColor(j, pixels11.Color(i, i, i));
  }
  pixels1.show();
  pixels2.show();
  pixels3.show();
  pixels4.show();
  pixels5.show();
  pixels6.show();
  pixels7.show();
  pixels8.show();
  pixels9.show();
  pixels10.show();
  pixels11.show();
  //FastLED.show(150);
  //}
  //for(int i = 255; i > 0; i--){
  i = 0;

  for (int j = 0; j < 119; j++) {
    if (j == 0) {
      pixels1.setPixelColor(j, pixels1.Color(i, i, i));
      pixels1.setPixelColor((119 - j), pixels1.Color(i, i, i));
      pixels2.setPixelColor(j, pixels2.Color(i, i, i));
      pixels2.setPixelColor((119 - j), pixels2.Color(i, i, i));
      pixels3.setPixelColor(j, pixels3.Color(i, i, i));
      pixels3.setPixelColor((119 - j), pixels3.Color(i, i, i));
      pixels4.setPixelColor(j, pixels4.Color(i, i, i));
      pixels4.setPixelColor((119 - j), pixels4.Color(i, i, i));
      pixels5.setPixelColor(j, pixels5.Color(i, i, i));
      pixels5.setPixelColor((119 - j), pixels5.Color(i, i, i));
      pixels6.setPixelColor(j, pixels6.Color(i, i, i));
      pixels6.setPixelColor((119 - j), pixels6.Color(i, i, i));
      pixels7.setPixelColor(j, pixels7.Color(i, i, i));
      pixels7.setPixelColor((119 - j), pixels7.Color(i, i, i));
      pixels8.setPixelColor(j, pixels8.Color(i, i, i));
      pixels8.setPixelColor((119 - j), pixels8.Color(i, i, i));
      pixels9.setPixelColor(j, pixels9.Color(i, i, i));
      pixels9.setPixelColor((119 - j), pixels9.Color(i, i, i));
      pixels10.setPixelColor(j, pixels10.Color(i, i, i));
      pixels10.setPixelColor((119 - j), pixels10.Color(i, i, i));
      pixels11.setPixelColor(j, pixels11.Color(i, i, i));
      pixels11.setPixelColor((119 - j), pixels11.Color(i, i, i));
    } else {
      pixels1.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels1.setPixelColor(j, pixels1.Color(i, i, i));
      pixels1.setPixelColor((119 - j), pixels1.Color(i, i, i));
      pixels1.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels2.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels2.setPixelColor(j, pixels2.Color(i, i, i));
      pixels2.setPixelColor((119 - j), pixels2.Color(i, i, i));
      pixels2.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels3.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels3.setPixelColor(j, pixels3.Color(i, i, i));
      pixels3.setPixelColor((119 - j), pixels3.Color(i, i, i));
      pixels3.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels4.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels4.setPixelColor(j, pixels4.Color(i, i, i));
      pixels4.setPixelColor((119 - j), pixels4.Color(i, i, i));
      pixels4.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels5.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels5.setPixelColor(j, pixels5.Color(i, i, i));
      pixels5.setPixelColor((119 - j), pixels5.Color(i, i, i));
      pixels5.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels6.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels6.setPixelColor(j, pixels6.Color(i, i, i));
      pixels6.setPixelColor((119 - j), pixels6.Color(i, i, i));
      pixels6.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels7.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels7.setPixelColor(j, pixels7.Color(i, i, i));
      pixels7.setPixelColor((119 - j), pixels7.Color(i, i, i));
      pixels7.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels8.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels8.setPixelColor(j, pixels8.Color(i, i, i));
      pixels8.setPixelColor((119 - j), pixels8.Color(i, i, i));
      pixels8.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels9.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels9.setPixelColor(j, pixels9.Color(i, i, i));
      pixels9.setPixelColor((119 - j), pixels9.Color(i, i, i));
      pixels9.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels10.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels10.setPixelColor(j, pixels10.Color(i, i, i));
      pixels10.setPixelColor((119 - j), pixels10.Color(i, i, i));
      pixels10.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
      pixels11.setPixelColor(j - 1, pixels1.Color(k, k, k));
      pixels11.setPixelColor(j, pixels11.Color(i, i, i));
      pixels11.setPixelColor((119 - j), pixels11.Color(i, i, i));
      pixels11.setPixelColor((119 - j + 1), pixels1.Color(k, k, k));
    }
    pixels1.show();
    pixels2.show();
    pixels3.show();
    pixels4.show();
    pixels5.show();
    pixels6.show();
    pixels7.show();
    pixels8.show();
    pixels9.show();
    pixels10.show();
    pixels11.show();
  }
  //FastLED.show();

  //}
  /*
for(int i = 0; i < 255; i++){
        for(int j = 0; j < 120; j++){
            pixels1.setPixelColor(j, pixels1.Color(0, i, 0));
pixels2.setPixelColor(j, pixels2.Color(0, i, 0));
        pixels3.setPixelColor(j, pixels3.Color(0, i, 0));
        pixels4.setPixelColor(j, pixels4.Color(0, i, 0));
        pixels5.setPixelColor(j, pixels5.Color(0, i, 0));
        pixels6.setPixelColor(j, pixels6.Color(0, i, 0));
        pixels7.setPixelColor(j, pixels7.Color(0, i, 0));
        pixels8.setPixelColor(j, pixels8.Color(0, i, 0));
        pixels9.setPixelColor(j, pixels9.Color(0, i, 0));
        pixels10.setPixelColor(j, pixels10.Color(0, i, 0));
        pixels11.setPixelColor(j, pixels11.Color(0, i, 0));


        setPixel(pixels1, j, 0, i, 0);
        setPixel(pixels2, j, 0, i, 0);
        setPixel(pixels3, j, 0, i, 0);
        setPixel(pixels4, j, 0, i, 0);
        setPixel(pixels5, j, 0, i, 0);
        setPixel(pixels6, j, 0, i, 0);
        setPixel(pixels7, j, 0, i, 0);
        setPixel(pixels8, j, 0, i, 0);
        setPixel(pixels9, j, 0, i, 0);
        setPixel(pixels10, j, 0, i, 0);
        setPixel(pixels11, j, 0, i, 0);
    }
        pixels1.show();
        pixels2.show();
        pixels3.show();
        pixels4.show();
        pixels5.show();
        pixels6.show();
        pixels7.show();
        pixels8.show();
        pixels9.show();
        pixels10.show();
        pixels11.show();}
    for(int i = 255; i > 0; i--){
        for(int j = 0; j < 120; j++)
        {
        pixels1.setPixelColor(j, pixels1.Color(0, i, 0));
pixels2.setPixelColor(j, pixels2.Color(0, i, 0));
        pixels3.setPixelColor(j, pixels3.Color(0, i, 0));
        pixels4.setPixelColor(j, pixels4.Color(0, i, 0));
        pixels5.setPixelColor(j, pixels5.Color(0, i, 0));
        pixels6.setPixelColor(j, pixels6.Color(0, i, 0));
        pixels7.setPixelColor(j, pixels7.Color(0, i, 0));
        pixels8.setPixelColor(j, pixels8.Color(0, i, 0));
        pixels9.setPixelColor(j, pixels9.Color(0, i, 0));
        pixels10.setPixelColor(j, pixels10.Color(0, i, 0));
        pixels11.setPixelColor(j, pixels11.Color(0, i, 0));

        setPixel(pixels1, j, 0, i, 0);
        setPixel(pixels2, j, 0, i, 0);
        setPixel(pixels3, j, 0, i, 0);
        setPixel(pixels4, j, 0, i, 0);
        setPixel(pixels5, j, 0, i, 0);
        setPixel(pixels6, j, 0, i, 0);
        setPixel(pixels7, j, 0, i, 0);
        setPixel(pixels8, j, 0, i, 0);
        setPixel(pixels9, j, 0, i, 0);
        setPixel(pixels10, j, 0, i, 0);
        setPixel(pixels11, j, 0, i, 0);
        delay(1);
        }

        pixels1.show();
        pixels2.show();
        pixels3.show();
        pixels4.show();
        pixels5.show();
        pixels6.show();
        pixels7.show();
        pixels8.show();
        pixels9.show();
        pixels10.show();
        pixels11.show();
    }
delay(1);
for(int i = 0; i < 255; i++){
        for(int j = 0; j < 120; j++)
        {
            pixels1.setPixelColor(j, pixels1.Color(0, 0, i));
pixels2.setPixelColor(j, pixels2.Color(0, 0, i));
        pixels3.setPixelColor(j, pixels3.Color(0, 0, i));
        pixels4.setPixelColor(j, pixels4.Color(0, 0, i));
        pixels5.setPixelColor(j, pixels5.Color(0, 0, i));
        pixels6.setPixelColor(j, pixels6.Color(0, 0, i));
        pixels7.setPixelColor(j, pixels7.Color(0, 0, i));
        pixels8.setPixelColor(j, pixels8.Color(0, 0, i));
        pixels9.setPixelColor(j, pixels9.Color(0, 0, i));
        pixels10.setPixelColor(j, pixels10.Color(0, 0, i));
        pixels11.setPixelColor(j, pixels11.Color(0, 0, i));
        setPixel(pixels1, j, 0, 0, i);
        setPixel(pixels2, j, 0, 0, i);
        setPixel(pixels3, j, 0, 0, i);
        setPixel(pixels4, j, 0, 0, i);
        setPixel(pixels5, j, 0, 0, i);
        setPixel(pixels6, j, 0, 0, i);
        setPixel(pixels7, j, 0, 0, i);
        setPixel(pixels8, j, 0, 0, i);
        setPixel(pixels9, j, 0, 0, i);
        setPixel(pixels10, j, 0, 0, i);
        setPixel(pixels11, j, 0, 0, i);
    }
        pixels1.show();
        pixels2.show();
        pixels3.show();
        pixels4.show();
        pixels5.show();
        pixels6.show();
        pixels7.show();
        pixels8.show();
        pixels9.show();
        pixels10.show();
        pixels11.show();
        }
    for(int i = 255; i > 0; i--){
        for(int j = 0; j < 120; j++)
        {
            pixels1.setPixelColor(j, pixels1.Color(0, 0, i));
pixels2.setPixelColor(j, pixels2.Color(0, 0, i));
        pixels3.setPixelColor(j, pixels3.Color(i, i, i));
        pixels4.setPixelColor(j, pixels4.Color(i, i, i));
        pixels5.setPixelColor(j, pixels5.Color(i, i, i));
        pixels6.setPixelColor(j, pixels6.Color(i, i, i));
        pixels7.setPixelColor(j, pixels7.Color(i, i, i));
        pixels8.setPixelColor(j, pixels8.Color(i, i, i));
        pixels9.setPixelColor(j, pixels9.Color(i, i, i));
        pixels10.setPixelColor(j, pixels10.Color(i, i, i));
        pixels11.setPixelColor(j, pixels11.Color(i, i, i));
        setPixel(pixels1, j, 0, 0, i);
        setPixel(pixels2, j, 0, 0, i);
        setPixel(pixels3, j, 0, 0, i);
        setPixel(pixels4, j, 0, 0, i);
        setPixel(pixels5, j, 0, 0, i);
        setPixel(pixels6, j, 0, 0, i);
        setPixel(pixels7, j, 0, 0, i);
        setPixel(pixels8, j, 0, 0, i);
        setPixel(pixels9, j, 0, 0, i);
        setPixel(pixels10, j, 0, 0, i);
        setPixel(pixels11, j, 0, 0, i);
        }
        pixels1.show();
        pixels2.show();
        pixels3.show();
        pixels4.show();
        pixels5.show();
        pixels6.show();
        pixels7.show();
        pixels8.show();
        pixels9.show();
        pixels10.show();
        pixels11.show();
    }*/
}
