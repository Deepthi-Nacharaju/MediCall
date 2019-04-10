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
#define PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

#include "Arduino.h"




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
#define lights_pin 9
#define haptic_pin 10
#define sound_pin 11
#define prox_pin 12
#define PLUG 2
#define nano A4
bool switch_status = 0;
#define pot A0
#define haptic_motor 6
int value = 0;
int effect = 19;
// Pot step 0.01265 Seconds
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);

void setup() {
  // put your setup code here, to run once:
  pinMode(signalPin, INPUT);
  pinMode(pot, INPUT);
  pinMode(lights_pin, INPUT);
  pinMode(haptic_pin, INPUT);
  pinMode(sound_pin, INPUT);
  pinMode(prox_pin, INPUT);
  pinMode(haptic_motor, OUTPUT);
  pinMode(PLUG, OUTPUT);
  Serial.begin(9600);
  Serial.println("Begin");
  // HAPTIC


  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command


  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
  // put your main code here, to run repeatedly:
  check_buttons();
  int value = analogRead(nano);
  if (value < 500){
    value = 0;
  }
  //Serial.println(value);
  read_pot = analogRead(pot);
  Serial.println(read_pot);
  call_delay_float = read_pot / 150;
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
      digitalWrite(PLUG, HIGH);
      if (HAPTIC) {
        analogWrite(haptic_motor, 255);
      }
      if (LIGHTS) {
        blinkLights();
        blinkLights();
        blinkLights();
      }
      analogWrite(haptic_motor, 0);
      signal_lost = 0;
      signal_switch = true;
      Serial.println("Call Placed");

      delay(1000);
      digitalWrite(PLUG, LOW);
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
      if (LIGHTS) {
        colorUpdate(strip.Color(255, 0, 0), 0);
      }
      vibe();
      //delay(100000);
    }
int value = analogRead(nano);
  if (value < 500){
    value = 0;
  }
    //Serial.println(value);
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
    analogWrite(haptic_motor, 75);
    delay(50);
    analogWrite(haptic_motor, 0);
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
