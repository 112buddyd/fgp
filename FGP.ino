#include "Hardware.h"
Hardware hw;

#include "Mole.h"
#include "Paint.h"

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Starting up.");
  randomSeed(analogRead(0));
  hw = Hardware();
  hw.displayStartup();
}

void loop() {
  hw.display.clearDisplay();
  hw.display.setCursor(0,0);
  hw.display.println("Select a game!");
  hw.display.println("Red: Mole");
  hw.display.println("Green: Paint");
  // hw.display.println("Blue: ????");
  hw.display.display();
  
  hw.leds[1].red = 255;
  hw.leds[4].green = 255;
  FastLED.show();
  
  char ckey = NO_KEY;
  while (ckey == NO_KEY) {
    ckey = hw.keypad.getKey();
  }
  int key = hw.key_to_led(ckey);
  switch (key) {
    case 1:
      Mole mole = Mole(hw);
      mole.run();
      break;
    case 4:
      Paint paint = Paint(hw);
      paint.run();
      break;
  }
}
