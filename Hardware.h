// Hardware.h
#ifndef Hardware_h
#define Hardware_h

// KEYPAD
#define ROW_COUNT 3
#define COL_COUNT 4

// OLED
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C 

// LED
#define LED_PIN A2
#define LED_COUNT 13
#define BRIGHTNESS 40
#define LED_TYPE WS2811 
#define COLOR_ORDER GRB
#define FPS 30

#include <Arduino.h>
#include <Keypad.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
// #include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FastLED.h"

class Hardware {
    private:
        byte row_pins[ROW_COUNT] = {4, 5, A3};
        byte col_pins[COL_COUNT] = {6, 7, 8, 9};
        char keys[ROW_COUNT][COL_COUNT] = {
          {'1', '2', '3', '4'}, 
          {'5', '6', '7', '8'},
          {'9', '0', 'A', 'B'},
        };
        
    public:
        Hardware();
        Keypad keypad = Keypad(makeKeymap(keys), row_pins, col_pins, ROW_COUNT, COL_COUNT);
        int key_to_led(char key);
        Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
        CRGB leds[LED_COUNT];
        CRGB colorOne = CRGB(0,255,0); // GREEN
        CRGB colorTwo = CRGB(255,0,0); // RED
        void displayStartup();
};

#endif