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
#include <Vector.h>

class Mole {
 public:
  Mole(Keypad keypad, CRGB leds[9], ezBuzzer buzzer, Adafruit_SSD1306 display);
  int run();
 private:
  Keypad keypad; 
  CRGB leds[9]; 
  ezBuzzer buzzer;
  Adafruit_SSD1306 display;
  int score;
  millisDelay moleTimers[9];
  millisDelay spawnTimer;
  int spawnSpeedMs;
  float spawnSpeedChange;
  int moleEscapeMs;
  float escapeSpeedChange;
  int roundNum;
  int hits;
  int lives;
  void renderMoles();
  void spawnRandomMole();
  void resetMole(int idx);
  void checkForMoleEscapes();
  void checkForMoleHits();
};
#endif
