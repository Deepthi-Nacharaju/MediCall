#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#define CAP1188_RESET  9
Adafruit_CAP1188 cap = Adafruit_CAP1188();
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

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
bool LIGHTS = false;
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
}

void loop() {
  // put your main code here, to run repeatedly:
  check_buttons();
  uint8_t value = cap.touched();
  read_pot = analogRead(pot);
  call_delay_float = read_pot * 0.0127;
  call_delay = call_delay_float * 1000;
  //Serial.println(call_delay);
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
        analogWrite(haptic_motor, 255);
      }
      if (LIGHTS) {
        blink_lights();
        blink_lights();
      }
      if (HAPTIC) {
        analogWrite(haptic_motor, 0);
      }
      signal_lost = 0;
      signal_switch = true;
      Serial.println("Call Placed");
      break;
    }
    if (millis() - timer > call_delay / 5 * 4) {
      if (LIGHTS) {
        digitalWrite(LED5, HIGH);
      }
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 3) {
      if (LIGHTS) {
        digitalWrite(LED4, HIGH);
      }
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 2) {
      if (LIGHTS) {
        digitalWrite(LED3, HIGH);
      }
      vibe();
    }
    else if (millis() - timer > call_delay / 5 * 1) {
      if (LIGHTS) {
        digitalWrite(LED2, HIGH);
      }
      vibe();
    }
    else if (millis() - timer > 10) {
      if (LIGHTS) {
        digitalWrite(LED1, HIGH);
      }
      vibe();
    }
    uint8_t value = cap.touched();
    //int value = digitalRead(signalPin);
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
  reset_lights();
  value = 0;
}

void reset_lights() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
}

void lights_on() {
  digitalWrite(LED5, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);
}
void blink_lights() {
  lights_on();
  delay(250);
  reset_lights();
  delay(250);
  lights_on();
  delay(250);
  reset_lights();
  delay(250);
}

void check_buttons() {
  switch_status = digitalRead(lights_pin);
  if (switch_status) {
    LIGHTS = true;
  }
  else {
    LIGHTS = false;
    reset_lights();
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
    drv.setWaveform(0, effect);  // play effect
    drv.setWaveform(1, 0);       // end waveform

    // play the effect!
    drv.go();
  }
}

