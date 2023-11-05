#include "FastLED.h"
#include "Paint.h"

const CRGB RED = CRGB(255,0,0);
const CRGB GREEN = CRGB(0,255,0);
const CRGB BLUE = CRGB(0,0,255);

Paint::Paint(Hardware hw) {
  hw = hw;
  FastLED.clear();
  FastLED.show();
  start = millis();
};

void Paint::changeColor(int idx) {
  if (hw.leds[idx] == RED) {
    hw.leds[idx] = GREEN;
  } else if (hw.leds[idx] == GREEN) {
    hw.leds[idx] = BLUE;
  } else if (hw.leds[idx] == BLUE) {
    hw.leds[idx] = RED;
  }
  FastLED.show();
};

int Paint::evalGameOver() {
  for (int i=0; i<LED_COUNT; i++) {
    if (hw.leds[i] != hw.leds[0]) {
      return 0;
    }
  }
  return (millis() - start) / 1000; 
};

CRGB Paint::getRandomColor() {
    int randInt = random(3);
    switch (randInt) {
      case 0:
        return RED;
        break;
      case 1:
        return GREEN;
        break;
      case 2:
        return BLUE;
        break;
    }
}

void Paint::run() {
  while (true) {
    char ckey = NO_KEY;
    while (ckey == NO_KEY) {
      ckey = hw.keypad.getKey();
    }
    int key = hw.key_to_led(ckey);
    changeColor(key);
    int score = evalGameOver();
    if (score > 0) {
      drawGameOverScreen(score);
      break;
    }
  }
}

void Paint::drawNewGameScreen() {
  hw.display.clearDisplay();
  hw.display.setCursor(0,0);
  hw.display.println("  PAINT!  ");
  hw.display.println(" Make all ");
  hw.display.println("the colors");
  hw.display.println("  match!  ");
  hw.display.display();
}

void Paint::drawGameOverScreen(int score) {
  hw.display.clearDisplay();
  hw.display.setCursor(0,0);
  hw.display.println("GREAT WORK");
  hw.display.println("Yout time:");
  hw.display.println(score);
  hw.display.println(" seconds  ");
  hw.display.display();
}