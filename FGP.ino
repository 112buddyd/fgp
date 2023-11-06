#include <Arduino.h>
#include <Keypad.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
// #include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FastLED.h"
#include <Vector.h>

// KEYPAD
#define ROW_COUNT 3
#define COL_COUNT 4
byte row_pins[ROW_COUNT] = {4, 5, A3};
byte col_pins[COL_COUNT] = {6, 7, 8, 9};
char keys[ROW_COUNT][COL_COUNT] = {
  {'1', '2', '3', '4'}, 
  {'5', '6', '7', '8'},
  {'9', '0', 'A', 'B'},
};
Keypad keypad = Keypad(makeKeymap(keys), row_pins, col_pins, ROW_COUNT, COL_COUNT);

// OLED
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LED
#define LED_PIN A2
#define LED_COUNT 13
#define BRIGHTNESS 40
#define LED_TYPE WS2811 
#define COLOR_ORDER GRB
#define FPS 30
CRGB leds[LED_COUNT];
CRGB colorOne = CRGB(0,255,0); // GREEN
CRGB colorTwo = CRGB(255,0,0); // RED    

int key_to_led(char key) {
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

void displayStartup() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("  Finn's  ");
  display.println(" Game Pad ");
  display.display();
  delay(2000);
}

// MOLE GAME
class Mole {
    private:
        int score;
        unsigned long moleTimers[LED_COUNT];
        unsigned long spawnTimer;
        int spawnSpeedMs;
        float spawnSpeedChange;
        int moleEscapeMs;
        float escapeSpeedChange;
        int roundNum;
        int hits;
        int lives;
        unsigned long lastFrame;

        bool mole_is_running(int idx);
        bool mole_is_stopped(int idx);
        bool mole_has_escaped(int idx);
        void renderMoles();
        void resetMole(int idx);
        void spawnRandomMole();
        void checkForMoleHits();
        void checkForMoleEscapes();
        void startAnimation();
        void endAnimation();
        void drawGameState();
        void drawNewGameScreen();
        void moleGameReset();

    public:
        Mole();
        void run();
};

Mole::Mole() {
  score = 0;
  moleTimers[LED_COUNT];
  spawnTimer;
  spawnSpeedMs = 1500;
  spawnSpeedChange = .9;
  moleEscapeMs = 3500;
  escapeSpeedChange = 1; // no change
  roundNum = 1;
  hits = 0;
  lives = 5;
  lastFrame = 0;
}


bool Mole::mole_is_running(int idx) {
  return !mole_is_stopped(idx) && (millis() - moleTimers[idx]) < moleEscapeMs;
}

bool Mole::mole_is_stopped(int idx) {
  return moleTimers[idx] == 0;
}

bool Mole::mole_has_escaped(int idx) {
  return !mole_is_stopped(idx) && (millis() - moleTimers[idx]) > moleEscapeMs;
}

void Mole::renderMoles(){
  // using interpolation color moles based on how close they are to expiring
  if ((millis() - lastFrame) < 1000 / FPS) {
    return;
  }
  for (int i=0; i<LED_COUNT; i++) {
    if (mole_is_running(i)) {
      unsigned long delta = millis() - moleTimers[i];
      leds[i].red = colorOne.red * (moleEscapeMs - delta) / moleEscapeMs + colorTwo.red * delta/ moleEscapeMs;
      leds[i].green = colorOne.green * (moleEscapeMs - delta) / moleEscapeMs + colorTwo.green * delta/ moleEscapeMs;
    } else {
      leds[i] = CRGB(0, 0, 0);
    }
  }
  lastFrame = millis();
  FastLED.show();
}

void Mole::spawnRandomMole(){
  // randomly choose to spawn new mole in empty hole and start timer
  int indexes[LED_COUNT];
  Vector<int> openPositions;
  openPositions.setStorage(indexes);
  for (int i=0; i<LED_COUNT; i++) {
    if (mole_is_stopped(i)) {
      openPositions.push_back(i);
    }
  }
  if (openPositions.size() == 0) {
    return;
  }
  int randInt = 0;
  while (randInt == 0) {
    // 0 is a special led and shouldn't be used in the game
    randInt = random(openPositions.size());
  }
  moleTimers[openPositions.at(randInt)] = millis();
}

void Mole::resetMole(int idx){
  // stop the mole's timer
  moleTimers[idx] = 0;
}

void Mole::checkForMoleHits(){
  // Get key and see if it matches an active mole
  char ckey = keypad.getKey();
  if (ckey == NO_KEY) {return;}
  int key  = key_to_led(ckey);
  if (mole_is_running(key)) {
    hits++;
    score++;
    resetMole(key);
  }
}

void Mole::checkForMoleEscapes(){
  // check to see if mole's timer timed out
  // if so decrement lives
  // then reset mole
  bool escape = false;
  for (int i=0; i<LED_COUNT; i++) {
    if (mole_has_escaped(i)) {
      lives--;
      resetMole(i);
      escape = true;
    }
  }
  if (escape) {
  }
  }

void Mole::moleGameReset() {
  score = 0;
  for (int i=0; i<LED_COUNT; i++) {
    moleTimers[i] = 0;
  }
  spawnTimer = 1;
  spawnSpeedMs = 1500;
  spawnSpeedChange = .9;
  moleEscapeMs = 3500;
  escapeSpeedChange = .9;
  roundNum = 1;
  hits = 0;
  lives = 5;
}

void Mole::startAnimation() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WHACKAMOLE");
  display.println(" GO GO GO ");
  display.display();
  fill_solid(leds, LED_COUNT, CRGB(255, 0, 0));
  FastLED.show();
  delay(750);
  fill_solid(leds, LED_COUNT, CRGB(255, 255, 0));
  FastLED.show();
  delay(750);
  fill_solid(leds, LED_COUNT, CRGB(0, 255, 0));
  FastLED.show();
  delay(1250);
}

void Mole::endAnimation() {
  int i = 0;
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("GAME OVER!");
  display.print("Score: ");
  display.println(score);
  display.display();

  fill_solid(leds, LED_COUNT, CRGB(255, 0, 0));
  FastLED.show();
  delay(500);
  fill_solid(leds, LED_COUNT, CRGB(0, 0, 0));
  FastLED.show();
  delay(250);
  fill_solid(leds, LED_COUNT, CRGB(255, 0, 0));
  FastLED.show();
  delay(500);
  fill_solid(leds, LED_COUNT, CRGB(0, 0, 0));
  FastLED.show();
  delay(250);
  fill_solid(leds, LED_COUNT, CRGB(255, 0, 0));
  FastLED.show();
  delay(500);
  fill_solid(leds, LED_COUNT, CRGB(0, 0, 0));
  FastLED.show();
  delay(250);
  
}

void Mole::run() {
  Serial.println("Starting Mole!");
  moleGameReset();
  // turn off all leds
  FastLED.clear();
  FastLED.show();

  startAnimation();

  // start loop, while lives > 0
  spawnTimer = millis();
  while (true) {
    while (hits < 10) {
      drawGameState();
      checkForMoleEscapes();
      if (lives <= 0) {
        endAnimation();
        return;
      }
      if (millis() - spawnTimer > spawnSpeedMs) {
        spawnRandomMole();
        spawnTimer = millis();
      }
      checkForMoleHits();
      renderMoles();
    }
    hits = 0;
    spawnSpeedMs = (int)(spawnSpeedMs * .8);
    moleEscapeMs = (int)(moleEscapeMs * .9);
    lives++;
    roundNum++;
  }
}

void Mole::drawGameState() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Score: ");
  display.println(score);
  display.print("Lives: ");
  display.println(lives);
  display.display();                       
}

void Mole::drawNewGameScreen() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WHACKAMOLE");
  display.println("Hit a key!");
  display.display();
}

// END MOLE GAME

// Paint.h
class Paint {
  private:
    unsigned long start;
    CRGB getRandomColor();
    void changeColor(int idx);
    int evalGameOver();
    void drawNewGameScreen();
    void drawGameOverScreen(int score);

  public:
    Paint();
    void run();
};

const CRGB RED = CRGB(255,0,0);
const CRGB GREEN = CRGB(0,255,0);
const CRGB BLUE = CRGB(0,0,255);

Paint::Paint() {
  start = millis();
  FastLED.clear();
  FastLED.show();
  for (int i=1; i<LED_COUNT; i++) {
    leds[i] = getRandomColor();
  }
  FastLED.show();
};

void Paint::changeColor(int idx) {
  if (leds[idx] == RED) {
    leds[idx] = GREEN;
  } else if (leds[idx] == GREEN) {
    leds[idx] = BLUE;
  } else if (leds[idx] == BLUE) {
    leds[idx] = RED;
  }
  FastLED.show();
};

int Paint::evalGameOver() {
  for (int i=1; i<LED_COUNT; i++) {
    if (leds[i] != leds[1]) {
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
  Serial.println("Starting paint!");
  drawNewGameScreen();
  while (true) {
    char ckey = NO_KEY;
    while (ckey == NO_KEY) {
      ckey = keypad.getKey();
    }
    int key = key_to_led(ckey);
    changeColor(key);
    int score = evalGameOver();
    if (score > 0) {
      drawGameOverScreen(score);
      break;
    }
  }
}

void Paint::drawNewGameScreen() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("  PAINT!  ");
  display.println(" Make all ");
  display.println("the colors");
  display.println("  match!  ");
  display.display();
}

void Paint::drawGameOverScreen(int score) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("GREAT WORK");
  display.println("Yout time:");
  display.println(score);
  display.println(" seconds  ");
  display.display();
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Starting up.");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setRotation(2);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  randomSeed(analogRead(0));

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  displayStartup();
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("What game?");
  display.println("R:    Mole");
  display.println("G:   Paint");
  display.println("B:     ???");
  display.display();
  
  leds[1].red = 255;
  leds[4].green = 255;
  FastLED.show();
  
  char ckey = NO_KEY;
  while (ckey == NO_KEY) {
    ckey = keypad.getKey();
  }
  int key = key_to_led(ckey);
  if (key == 1) {
    Mole mole = Mole();
    mole.run();
  } else if (key == 4) {
    Paint paint = Paint();
    paint.run();
  }
}
