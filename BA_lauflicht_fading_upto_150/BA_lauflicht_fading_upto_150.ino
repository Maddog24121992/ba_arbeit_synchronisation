//#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        40
#define INTERRUPTPIN 40
#define NUMBPIXELS 60
#include <stdio.h>
//#include "esp_system.h"
#include "driver/gpio.h"
#ifdef ESP32
#include <esp32-hal-timer.h>
#endif
//#include <Ethernet.h>

//Adafruit_NeoPixel pixels(NUMBPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel pixels2(NUMBPIXELS, PIN2, NEO_GRB + NEO_KHZ800);
#define DELAYVALSHORT 10
#define DELAYVALMID 50
#define DELAYVALLONG 10
#define DEVLEDVAL 1
int isLoopFree = 0;
int isLoop2Free = 1;
hw_timer_t * myTimer = NULL;
int args = 10000000;
void sync(){
  //esp_intr_alloc();
}
void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  //pixels.begin();
  //pixels2.begin();
 pinMode(INTERRUPTPIN, OUTPUT);
 
 myTimer = timerBegin(40000000);
 timerStart(myTimer);
 timerAttachInterrupt(myTimer, interrupt);
  timerAlarm(myTimer, args, true, 0);
}

void interrupt(){
  digitalWrite(INTERRUPTPIN, HIGH);
  delay(10000);
  digitalWrite(INTERRUPTPIN, LOW);
  timerRestart(myTimer);
}

void loop(){}/*
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN 5
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin(); // Initialize NeoPixel strip
}

void loop() {
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Red color
    pixels.show();
    delay(100);
  }
}*/
/*void fade(Adafruit_NeoPixel pixelStripe, int counter, int ledVal) 
{
  while(counter > 0){#include <WiFi.h>

void setup(){
  Serial.begin(115200);
  
  // Variable to store the MAC address
  uint8_t baseMac[6];
  
  // Get MAC address of the WiFi station interface
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  Serial.print("Station MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);
  
  // Get the MAC address of the Wi-Fi AP interface
  esp_read_mac(baseMac, ESP_MAC_WIFI_SOFTAP);
  Serial.print("SoftAP MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);
  
  // Get the MAC address of the Bluetooth interface
  esp_read_mac(baseMac, ESP_MAC_BT);
  Serial.print("Bluetooth MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);

  // Get the MAC address of the Ethernet interface
  esp_read_mac(baseMac, ESP_MAC_ETH);
  Serial.print("Ethernet MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);
}
 
void loop(){
}
  for(int j=0; j<ledVal; j++) {
    
    for(int i=0; i<NUMBPIXELS; i++){
      pixelStripe.setPixelColor(i, pixelStripe.Color(j, j, j));
    }
    pixelStripe.show();
    delay(1);
  }
  delay(100000);
  for(int j=ledVal; j>=0; j--) {
      
    for(int i=0; i<NUMBPIXELS; i++){
      pixelStripe.setPixelColor(i, pixelStripe.Color(j, j, j));
    }
    pixelStripe.show();
    delay(1);
  }
  counter--;
  }
}
/*
void hardwareTestFade(int ledVal, int delayTime, bool firstRun){
  
    for(int k = 0; k < 1; k++)
    {
      for(int i=0; i<ledVal; i++){
        for(int j = 0; j < NUMBPIXELS; j++)
        {
          pixels.setPixelColor(j, pixels.Color(i, i, i));
        }
        pixels.show();
      }150
      delay(UINT32_MAX);
      delay(4000);
      for(int i=ledVal; i>=0; i--){
        for(int j = 0; j < NUMBPIXELS; j++)
        {
          pixels.setPixelColor(j, pixels.Color(i, i, i));
        }
        pixels.show();
      }
      delay(1000);
    }
    for(int i = 0; i < NUMBPIXELS; i++)
    {
      int j = 0;
      for(j ; j <= ledVal; j++)
      {
        pixels.setPixelColor(i, pixels.Color(j, j, j));
        pixels.show();
      }
      delay(250);
      for(j ; j >= 0; j --)
      {
        pixels.setPixelColor(i, pixels.Color(j, j, j));
        pixels.show();
      }
      delay(500);
    }
    delay(2000);
  
}

void runningLights(int counter){
  while(counter > 0){
  for(int i = 0; i < NUMBPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(DEVLEDVAL, 0, 0));
    pixels.show();
    delay(DELAYVALLONG);
  }    for(k = 0; k < NUMBPIXELS; ){

  for(int i = 0; i < NUMBPIXELS; i++){150
    pixels.setPixelColor(i, pixels.Color(DEVLEDVAL, DEVLEDVAL, 0));
    pixels.show();
    delay(DELAYVALLONG);
  }
  for(int i = 0; i < NUMBPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, DEVLEDVAL, 0));
    pixels.show();
    delay(DELAYVALLONG);
  }
  for(int i = 0; i < NUMBPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, DEVLEDVAL, DEVLEDVAL));
    pixels.show();
    delay(DELAYVALLONG);
  }
  for(int i = 0; i < NUMBPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, DEVLEDVAL));
    pixels.show();
    delay(DELAYVALLONG);
  }
  for(int i = 0; i < NUMBPIXELS; i++){Adafruit_Neopixel pixelStr150ipe
    pixels.setPixelColor(i, pixels.Color(DEVLEDVAL, 0, DEVLEDVAL));
    pixels.show();
    delay(DELAYVALLONG);
  }
  for(int i = 0; i < NUMBPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(DELAYVALLONG);
  }
  //pixels.clear();
  counter--;
  }
}

void rgb_aufbau(int counter){
  while(counter>0){
  for(int k = 0; k < NUMBPIXELS; ){
    for(int i=0; i<NUMBPIXELS-k; i++){
      pixels.setPixelColor(i, pixels.Color(DEVLEDVAL, 0, 0));
      pixels.show();
      delay(DELAYVALLONG);
    }
    k++;
    for(int i=0; i<NUMBPIXELS-k; i++){
      pixels.setPixelColor(i, pixels.Color(0, DEVLEDVAL, 0));
      pixels.show();
      delay(DELAYVALLONG);
    }
    k++;
    for(int i=0; i<NUMBPIXELS-k; i++){
      pixels.setPixelColor(i, pixels.Color(0, 0, DEVLEDVAL));
      pixels.show();
      delay(DELAYVALLONG);
    }
    k++;
  }
    delay(2000);
    for(int i=0; i<NUMBPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(200);
    }
    counter--;
  }
}


void rgb_aufbau_einzeln(int counter){
  while(counter > 0){
    for(int i=0; i<NUMBPIXELS-k; i++){
      pixels.setPixelColor(i, pixels.Color(DEVLEDVAL, 0, 0));
      pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVALLONG);
    }
    k++;
    for(int i=0; i<NUMBPIXELS-k; i++){
}

void runningLights(int counter){
      pixels.setPixelColor(i, pixels.Color(0, DEVLEDVAL, 0));
      pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVALLONG);
    }
    k++;
    for(int i=0; i<NUMBPIXELS-k; i++){
      pixels.setPixelColor(i, pixels.Color(0, 0, DEVLEDVAL));
      pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVALLONG);
    }
    k++;
  }
  for(int i = NUMBPIXELS; i >=0; ){
    for(int j=i; j<NUMBPIXELS; j++){
      pixels.setPixelColor(j+1, pixels.Color(DEVLEDVAL, 0, 0));
      pixels.setPixelColor(j, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVALLONG);
    }
    i--;
    for(int j=i; j<NUMBPIXELS; j++){
      pixels.setPixelColor(j+1, pixels.Color(0,#include <Ethernet> DEVLEDVAL, 0));
      pixels.setPixelColor(j, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVALLONG);
    }
    i--;for(int j=i; j<NUMBPIXELS; j++){
      pixels.setPixelColor(j+1, pixels.Color(0, 0, DEVLEDVAL));
      pixels.setPixelColor(j, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVALLONG);
    }
    i--;
  }
  counter--;
  }
}

void loopA() {
  for(int i = 29; i > 0; i--){
    pixels.setPixelColor(i, pixels.Color(150, 150, 150));
    if(i<30){
      pixels.setPixelColor(i+1, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(300);
  }
}
    for(k = 0; k < NUMBPIXELS; ){

void loopB(){
  for(int i = 30; i < NUMBPIXELS-1; i++){
    pixels.setPixelColor(i, pixels.Color(150, 150, 150));
    if(i>30){
      pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(300);
}

void runningLights(int counter){
  }
}

void runningLights(int counter){
}
void loop2() {
  isLoopFree = 1;
  runningLights(2);
  delay(100);

  loopA();
  
}

/*void vortragssequenz()
{
  fade(2, 150);
  delay(100);
  runningLights(2);
  delay(100);
  rgb_aufbau(1);
  delay(100);    
}*/
//void loop() {
//hardwareTestFade(200, 0, false);  //vortragssequenz();
  //hardwareTestFade(255, 15000, true);
  //loopB()
  //uint8_t mac[6];
  //esp_read_mac
  //fade(pixels, 4, 255);
  //fade(pixels2, 4, 255);
  //}
  /*
  rgb_aufbau_einzeln(1);
  delay(100);*/
//#include <WiFi.h>

//void setup(){
  //Serial.begin(115200);
  
  // Variable to store the MAC address
  //uint8_t baseMac[6];
  
  // Get MAC address of the WiFi station interface
  /*esp_read_mac(baseMac, ESP_IF_WIFI_STA);
  Serial.print("Station MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);
  
  // Get the MAC address of the Wi-Fi AP interface
  esp_read_mac(baseMac, ESP_MAC_WIFI_SOFTAP);
  Serial.print("SoftAP MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);
  
  // Get the MAC address of the Bluetooth interface
  esp_read_mac(baseMac, ESP_MAC_BT);
  Serial.print("Bluetooth MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);

  // Get the MAC address of the Ethernet interface
  esp_read_mac(baseMac, ESP_MAC_ETH);
  Serial.print("Ethernet MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);*/
//}
 
//void loop(){
//}
