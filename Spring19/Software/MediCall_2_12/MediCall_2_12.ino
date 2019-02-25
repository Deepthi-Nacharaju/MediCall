#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#define CAP1188_RESET  9
Adafruit_CAP1188 cap = Adafruit_CAP1188();
#include "Adafruit_DRV2605.h"
/* This is packages necessary for DFPlayer
  #include "Arduino.h"
  #include "SoftwareSerial.h"
  #include "DFRobotDFPlayerMini.h"
*/

Adafruit_DRV2605 drv;

SoftwareSerial mySoftwareSerial(7, 8); // DF pin 2 goes to 11, DF pin 3 goes to 10
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
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
#define lights_pin 6
#define haptic_pin 7
#define sound_pin 8
#define prox_pin 9
bool switch_status = 0;
#define pot A0
#define haptic_motor 3
int value = 0;
int effect = 19;
// Pot step 0.01265 Seconds
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);

void setup() {
  // put your setup code here, to run once:
  pinMode(pot, INPUT);
  pinMode(lights_pin, INPUT);
  pinMode(haptic_pin, INPUT);
  pinMode(sound_pin, INPUT);
  pinMode(prox_pin, INPUT);
  pinMode(haptic_motor, OUTPUT);
  Serial.begin(115200);
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

  // This is the set up code from DFPlayer_test
  mySoftwareSerial.begin(9600);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(30);  //Set volume value. From 0 to 30

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
      break;
    }
    if (millis() - timer > call_delay / 5 * 4) {
      if (LIGHTS) {
        colorUpdate(red, 4);
      }
      vibe();
      load_sound();
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
      load_sound();
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
  /* This is the loop code for the DFPlayer.
    static unsigned long timer = millis();

    if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
    }

    if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
    }
  */
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

// Below is the code for the DFPlayer
void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
void load_sound() {
  if (SOUND) {
    myDFPlayer.play(1);  //Play the first mp3
    if (myDFPlayer.available()) {
      printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
    }
  }
}

