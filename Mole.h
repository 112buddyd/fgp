#ifndef Mole_h
#define Mole_h

#include <Keypad.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ezBuzzer.h>
#include <millisDelay.h>

class Mole {
 public:
  Mole(CRGB leds, Keypad keypad, Adafruit_SSD1306 display, ezBuzzer buzzer, byte size);
  void run();
};
#endif
