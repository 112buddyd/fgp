#include <Keypad.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ezBuzzer.h>
#include "Mole.h"

// KEYS
const byte row_count = 3;
const byte col_count = 3;
byte row_pins[row_count] = {4, 16, 17};
byte col_pins[col_count] = {18, 19, 23};
char keys[row_count][col_count] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
};
Keypad keypad = Keypad(makeKeymap(keys), row_pins, col_pins, row_count, col_count);

// OLED
#define SDA_PIN 21
#define SCL_PIN 22
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin or -1 if sharing Arduino reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LED
#define LED_PIN 13
#define LED_COUNT 9
#define BRIGHTNESS 40
#define LED_TYPE WS2811 
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
CRGB leds[LED_COUNT];

// BUZZER
#define BUZZER_PIN 14
ezBuzzer buzzer(BUZZER_PIN);

void setup() {
  Serial.begin(115200);
  delay(3000); // power on safety delay

  // LED SETUP
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // OLED SETUP
  display.display(); // shows adafruit splash
  delay(1500);
  display.clearDisplay();

  // BUZZER SETUP
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  buzzer.loop(); // MUST call the buzzer.loop() function in loop()
  
  Mole mole = Mole(keypad, leds, buzzer, display);
  mole.run(); 
}
