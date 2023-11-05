#include <Keypad.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Vector.h>

// KEYS
const byte row_count = 3;
const byte col_count = 4;
byte row_pins[row_count] = {4, 5, A3};
byte col_pins[col_count] = {6, 7, 8, 9};
char keys[row_count][col_count] = {
  {'1', '2', '3', '4'}, 
  {'5', '6', '7', '8'},
  {'9', '0', 'A', 'B'},
};
Keypad keypad = Keypad(makeKeymap(keys), row_pins, col_pins, row_count, col_count);

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

// OLED
// #define SDA_PIN 21
// #define SCL_PIN 22
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LED
#define LED_PIN A2
#define LED_COUNT 13
#define BRIGHTNESS 40
#define LED_TYPE WS2811 
#define COLOR_ORDER GRB
#define FPS 30
unsigned long lastFrame = 0;
CRGB leds[LED_COUNT];
CRGB colorOne = CRGB(0,255,0); // GREEN
CRGB colorTwo = CRGB(255,0,0); // RED


// MOLE GAME
int score = 0;
unsigned long moleTimers[LED_COUNT];
unsigned long spawnTimer;
int spawnSpeedMs = 1500;
float spawnSpeedChange = .9;
int moleEscapeMs = 3500;
float escapeSpeedChange = 1; // no change
int roundNum = 1;
int hits = 0;
int lives = 5;

bool mole_is_running(int idx) {
  return !mole_is_stopped(idx) && (millis() - moleTimers[idx]) < moleEscapeMs;
}

bool mole_is_stopped(int idx) {
  return moleTimers[idx] == 0;
}

bool mole_has_escaped(int idx) {
  return !mole_is_stopped(idx) && (millis() - moleTimers[idx]) > moleEscapeMs;

}

void renderMoles(){
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

void spawnRandomMole(){
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
void resetMole(int idx){
  // stop the mole's timer
  moleTimers[idx] = 0;
}

void checkForMoleHits(){
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

void checkForMoleEscapes(){
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

void moleGameReset() {
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

void displayStartup() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("  Finn's  ");
  display.println(" Game Pad ");
  display.display();
  delay(2000);
}

void startAnimation() {
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

void endAnimation() {
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

int run() {
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
        return score;
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

void drawGameState() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Score: ");
  display.println(score);
  display.print("Lives: ");
  display.println(lives);
  display.display();                       
}

void drawNewGameScreen() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WHACKAMOLE");
  display.println("Hit a key!");
  display.display();
}

// END MOLE GAME

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

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

  displayStartup();
}

void loop() {
  Serial.println("Waiting to start...");
  char key = keypad.getKey();
  while (key == NO_KEY) {
    drawNewGameScreen();
    key = keypad.getKey();
  }
  moleGameReset();
  run();
  delay(3000);
}