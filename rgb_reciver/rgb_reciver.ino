#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_attr.h"

#define INTERRUPT_PIN  GPIO_NUM_17     // Eingangspin für Interrupt
#define STRIPE1 GPIO_NUM_0
#define STRIPE2 GPIO_NUM_1
#define STRIPE3 GPIO_NUM_2
#define STRIPE4 GPIO_NUM_3
#define STRIPE5 GPIO_NUM_15
#define STRIPE6 GPIO_NUM_16
#define STRIPE7 GPIO_NUM_18
#define STRIPE8 GPIO_NUM_21
#define STRIPE9 GPIO_NUM_45
#define STRIPE10 GPIO_NUM_46
#define STRIPE11 GPIO_NUM_47

#define NUMBPIXELS 120


int i;
static const char *TAG = "ISR_QUEUE";

static QueueHandle_t gpio_evt_queue = NULL;
static int pin_level_glob = 0;
hw_timer_t * myTimer = NULL;
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


void setPixel(Adafruit_NeoPixel stripe, int pixelIndex, int red, int blue, int green){
  int pixelCorrespondIndex = 119-pixelIndex;
  stripe.setPixelColor(pixelIndex, stripe.Color(red, blue, green));
  stripe.setPixelColor(pixelCorrespondIndex, stripe.Color(red, blue, green));
  stripe.show();
}

// ISR – wird bei steigender Flanke auf IN_GPIO ausgelöst
void IRAM_ATTR gpio_isr_handler(void *arg) {
    //ESP_LOGI(TAG,"ISR ausgelöst! GPIO: "); // Debug-Log für Interrupt
    if(xQueueSendFromISR(gpio_evt_queue, arg, NULL)) {
      
      //ESP_LOGI(TAG, "Event in Queue");  // → Event in Queue
    }  else {
      //ESP_LOGI(TAG, "Queue full, event not sent");  // → Queue voll
    }
}
void loop(){}
// Task – reagiert auf Events aus der ISR
void gpio_event_task(void *arg) {
    // uint32_t io_num;
    int pin_level;
    while (1) {
      // i = gpio_get_level(IN_GPIO);
      if (xQueueReceive(gpio_evt_queue, &pin_level, pdMS_TO_TICKS(10))) {
          //ESP_LOGI(TAG, "queue received pin level %d. triggering OUT_GPIO...", pin_level);
          ESP_LOGI(TAG, "the internal clock had run %d", timerReadMicros(myTimer));
          timerRestart(myTimer);
      } else {
        //ESP_LOGI(TAG, "Keine Events in der Queue");
      }
    }
}

void pin_level_task(void *arg) {
    while (1) {
        int pin_level = gpio_get_level(IN_GPIO);
        //ESP_LOGI(TAG, "Pin-Level IN_GPIO: %d", pin_level);
        if(pin_level && !pin_level_glob) {
          // rising edge detected
          ESP_LOGI(TAG, "RISING");
          if(xQueueSend(gpio_evt_queue, &pin_level, 0))
          {
             // ESP_LOGI(TAG, "Pin-Level in Queue");
          } else {
             // ESP_LOGI(TAG, "Queue full, pin level not sent");
          }
        }
        pin_level_glob = pin_level;

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
gpio_config_t io_conf1 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 0
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf2 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 1
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf3 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 2
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf4 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 3
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf5 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 15
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf6 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 16
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf7 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 17
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf8 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 18
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf9 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 21
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf10 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 45
        .mode = GPIO_MODE_OUTPUT,                // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config_t io_conf11 = {
        .pin_bit_mask = 1ULL << GPIO_OUTPUT_PIN, // Bitmask for GPIO 46
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
    // Konfiguriere IN_GPIO (Eingang, Interrupt auf steigende Flanke)
    gpio_config_t io_conf_in = {
        .pin_bit_mask = (1ULL << IN_GPIO),
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
    
    // ISR-Service installieren (IRAM-Modus!)
    // esp_err_t ret = gpio_install_isr_service(ESP_INTR_FLAG_LEVELMASK |ESP_INTR_FLAG_SHARED| ESP_INTR_FLAG_EDGE | ESP_INTR_FLAG_IRAM);
   // esp_err_t ret = gpio_install_isr_service(ESP_INTR_FLAG_LEVELMASK);
    // ESP_LOGI(TAG, "install_isr_service: %s", esp_err_to_name(ret));
    // gpio_isr_handler_add(IN_GPIO, gpio_isr_handler, (void *) IN_GPIO);
    // gpio_intr_enable(IN_GPIO);
    // Starte Event-Task
    xTaskCreate(gpio_event_task, "gpio_event_task", 4096, NULL, 10, NULL);

    // Starte Pin-Level-Task
    xTaskCreate(pin_level_task, "pin_level_task", 4096, NULL, 5, NULL);
    //ESP_LOGI(TAG, "GPIO Interrupt auf steigende Flanke aktiviert");


    //ESP_LOGI(TAG, "GPIO Interrupt-Setup abgeschlossen");

    
}

void sequence(){
  
}