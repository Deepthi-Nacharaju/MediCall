#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#define CAP1188_RESET  9
Adafruit_CAP1188 cap = Adafruit_CAP1188();
#include "Adafruit_DRV2605.h"


Adafruit_DRV2605 drv;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);


int LED1 = 30;
int LED2 = 32;
int LED3 = 34;
int LED4 = 36;
int LED5 = 38;
int signalPin = 52;
int counter = 1;
unsigned long timer = 0;
bool signal_switch = false;
unsigned long signal_lost = 0;
unsigned long read_pot = 0;
float call_delay_float = 0.0000;
int call_delay = 0;
bool LIGHTS = true;
bool HAPTIC = false;
bool SOUND = false;
bool PROX = false;
#define lights_pin 42
#define haptic_pin 44
#define sound_pin 46
#define prox_pin 48
bool switch_status = 0;
#define pot A0
#define haptic_motor 2
int value = 0;
int effect = 19;
// Pot step 0.01265 Seconds
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(signalPin, INPUT);
  pinMode(pot, INPUT);
  pinMode(lights_pin, INPUT);
  pinMode(haptic_pin, INPUT);
  pinMode(sound_pin, INPUT);
  pinMode(prox_pin, INPUT);
  pinMode(haptic_motor, OUTPUT);
  Serial.begin(9600);
  Serial.println("CAP1188 test!");

  // Initialize the sensor, if using i2c you can pass in the i2c address
  // if (!cap.begin(0x28)) {
  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");
  // HAPTIC
  drv.begin();

  drv.selectLibrary(1);

  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // put your main code here, to run repeatedly:
  check_buttons();
  uint8_t value = cap.touched();
  read_pot = analogRead(pot);
  call_delay_float = read_pot * 0.0127;
  call_delay = call_delay_float * 1000;
  while (value) {
    check_buttons();
    if (counter) {
      timer = millis();
      counter = 0;
      //Serial.println(timer);
      //Serial.println(counter);
    }
    if (millis() - timer > call_delay) {
      if (HAPTIC) {
        analogWrite(11, 255);
      }
      if (LIGHTS) {
        blinkLights();
        blinkLights();
        blinkLights();
      }
      analogWrite(11, 0);
      signal_lost = 0;
      signal_switch = true;
      Serial.println("Call Placed");
      break;
    }
    if (millis() - timer > call_delay / 5 * 4) {
      if (LIGHTS) {
        colorUpdate(red, 4);
      }
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 3) {
      if (LIGHTS) {
        colorUpdate(red, 3);
      }
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 2) {
      if (LIGHTS) {
        colorUpdate(red, 2);
      }
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 1) {
      if (LIGHTS) {
        colorUpdate(red, 1);
      }
      vibe();
    }
    else if (millis() - timer > 10) {
      Serial.print("LIGHTS:");
      Serial.println(LIGHTS);
      if (LIGHTS) {
        colorUpdate(strip.Color(255, 0, 0), 0);
      }
      vibe();
      //delay(100000);
    }
    uint8_t value = cap.touched();
    if (!value && signal_switch == true) {
      signal_lost = millis();
      signal_switch = false;
    }
    if (value) {
      signal_lost = 0;
      signal_switch = true;
    }
    if (millis() - signal_lost > 500 && signal_switch == false) {
      break;
    }
    //Serial.println(value);
  }
  counter = 1;
  value = 0;
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
  }
}

void check_buttons() {
  switch_status = digitalRead(lights_pin);
  if (switch_status) {
    LIGHTS = true;
  }
  else {
    LIGHTS = false;
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
      strip.show();
    }
  }
  switch_status = digitalRead(haptic_pin);
  if (switch_status) {
    HAPTIC = true;
  }
  else {
    HAPTIC = false;
    analogWrite(haptic_motor, 0);
  }
  switch_status = digitalRead(sound_pin);
  if (switch_status) {
    SOUND = true;
  }
  else {
    SOUND = false;
  }
  switch_status = digitalRead(prox_pin);
  if (switch_status) {
    PROX = true;
  }
  else {
    PROX = false;
  }
}

void vibe() {
  if (HAPTIC) {
    analogWrite(11, 75);
    delay(50);
    analogWrite(11, 0);
  }
}

void colorUpdate(uint32_t c, int pixel) {
  strip.setPixelColor(pixel, c);
  strip.show();
}

void blinkLights() {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, green);
    strip.show();
  }
  delay(500);
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
  }
}

