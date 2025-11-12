// Copyright 2024 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @brief This example demonstrates usage of RGB LED driven by RMT
 *
 * The output is a visual WS2812 RGB LED color moving in a 8 x 4 LED matrix
 * Parameters can be changed by the user. In a single LED circuit, it will just blink.
 */

// The effect seen in (Espressif devkits) ESP32C6, ESP32H2, ESP32C3, ESP32S2 and ESP32S3 is like a Blink of RGB LED
#ifdef PIN_LED_RGB
#define BUILTIN_RGBLED_PIN PIN_LED_RGB
#else
#define BUILTIN_RGBLED_PIN 35  // ESP32 has no builtin RGB LED (PIN_LED_RGB)
#endif
#include "esp32-hal-cpu.h" // Diese Bibliothek enthält die Funktion


//
// Note: This example uses a board with 32 WS2812b LEDs chained one
//      after another, each RGB LED has its 24 bit value
//      for color configuration (8b for each color)
//
//      Bits encoded as pulses as follows:
//
//      "0":
//         +-------+              +--
//         |       |              |
//         |       |              |
//         |       |              |
//      ---|       |--------------|
//         +       +              +
//         | 0.4us |   0.85 0us   |
//
//      "1":
//         +-------------+       +--
//         |             |       |
//         |             |       |
//         |             |       |
//         |             |       |
//      ---+             +-------+
//         |    0.8us    | 0.4us |

#define NR_OF_LEDS     60                   // 60 RGB-LEDs
#define NR_OF_ALL_BITS (24 * NR_OF_LEDS)   // 24 Bit pro LED

rmt_data_t led_data[NR_OF_ALL_BITS];       // RMT-Signaldaten

void setup() {
  Serial.begin(115200);
  setCpuFrequencyMhz(240);
  if (!rmtInit(BUILTIN_RGBLED_PIN, RMT_TX_MODE, RMT_MEM_NUM_BLOCKS_4, 10000000)) {
    Serial.println("init sender failed\n");
  }
  Serial.println("RMT init erfolgreich (12,5 ns tick)\n");
}

// Farbe: Weiß (R=255, G=255, B=255)
const uint8_t color[3] = {0xFF, 0xFF, 0xFF};  // G, R, B

int led_index = 0;

void loop() {
  int led, col, bit;
  int i = 0;
  for (led = 0; led < NR_OF_LEDS; led++) {
    for (col = 0; col < 3; col++) {
      for (bit = 0; bit < 8; bit++) {
        if ((color[col] & (1 << (7 - bit))) && (led == led_index)) {
          led_data[i].level0 = 1;
          led_data[i].duration0 = 8;
          led_data[i].level1 = 0;
          led_data[i].duration1 = 4;
        } else {
          led_data[i].level0 = 1;
          led_data[i].duration0 = 4;
          led_data[i].level1 = 0;
          led_data[i].duration1 = 8;
        }
        String ausgabe = "led nr. " + String(led) +" has \n level0 "+String(led_data[i].level0)+",\n duration0 "+String(led_data[i].duration0)+", \n level1 "+String(led_data[i].level1)+" and \n duration1 "+String(led_data[i].duration1)+" for\n bit "+String(bit)+"at color"+String(col);
        Serial.println(ausgabe);
        i++;
      }
    }
  }

  // Nächste LED aktivieren
  led_index = (led_index + 1) % NR_OF_LEDS;
  // Signal an LEDs senden
  rmtWrite(BUILTIN_RGBLED_PIN, led_data, NR_OF_ALL_BITS, RMT_WAIT_FOR_EVER);
  
  delay(1);  // Geschwindigkeit der Animation
}