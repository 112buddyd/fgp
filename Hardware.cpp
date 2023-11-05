#include "Hardware.h"

Hardware::Hardware() {   
    Serial.println("starting up hardware.");
    // OLED SETUP
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    }
    display.setRotation(2);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    
    // LED SETUP
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
}

int Hardware::key_to_led(char key) {
  switch (key) {
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case '0':
      return 10;
    case 'A':
      return 11;
    case 'B':
      return 12;
  }
}

void Hardware::displayStartup() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("  Finn's  ");
  display.println(" Game Pad ");
  display.display();
  delay(2000);
}