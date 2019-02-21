#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#define CAP1188_RESET  9
Adafruit_CAP1188 cap = Adafruit_CAP1188();
#include "Adafruit_DRV2605.h"
Adafruit_DRV2605 drv;
int signalPin = 52;
int counter = 1;
unsigned long timer = 0;
bool signal_switch = false;
unsigned long signal_lost = 0;
unsigned long read_pot = 0;
float call_delay_float = 0.0000;
int call_delay = 0;
#define sound_pin 46
#define prox_pin 48
bool switch_status = 0;
#define pot A0
int value = 0;
int effect = 19;
// Pot step 0.01265 Seconds

void setup() {
  // put your setup code here, to run once:
  pinMode(pot, INPUT);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t value = cap.touched();
  read_pot = analogRead(pot);
  call_delay_float = read_pot * 0.0127;
  call_delay = call_delay_float * 1000;
  Serial.println(value);
  while (value) {
    if (counter) {
      timer = millis();
      counter = 0;
    }
    if (millis() - timer > call_delay) {
      signal_lost = 0;
      signal_switch = true;
      Serial.println("Call Placed");
      break;
    }
    if (millis() - timer > call_delay / 5 * 4) {
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 3) {
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 2) {
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 1) {
      vibe();
    }
    else if (millis() - timer > 10) {
      vibe();
    }
    uint8_t value = cap.touched();
    Serial.println(value);
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
  }
  counter = 1;
  value = 0;
}


void vibe() {
  drv.setWaveform(0, effect);  // play effect
  drv.setWaveform(1, 0);       // end waveform
  // play the effect!
  drv.go();
}

