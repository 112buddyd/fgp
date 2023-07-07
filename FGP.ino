#include <Keypad.h>
#include <FastLED.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #include <ezBuzzer.h>
#include <Vector.h>

// KEYS
const byte row_count = 3;
const byte col_count = 3;
byte row_pins[row_count] = {4, 16, 17};
byte col_pins[col_count] = {19, 22, 23};
char keys[row_count][col_count] = {
  {'2', '3', '8'},
  {'1', '4', '7'},
  {'0', '5', '6'}
};
Keypad keypad = Keypad(makeKeymap(keys), row_pins, col_pins, row_count, col_count);

// OLED
// #define SDA_PIN 21
// #define SCK_PIN 18ß
// #define SCREEN_WIDTH 128 
// #define SCREEN_HEIGHT 32
// #define OLED_RESET -1 // Reset pin or -1 if sharing Arduino reset pin
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LED
#define LED_PIN 13
#define LED_COUNT 9
#define BRIGHTNESS 40
#define LED_TYPE WS2811 
#define COLOR_ORDER GRB
CRGB leds[LED_COUNT];

// BUZZER
// #define BUZZER_PIN 14
// ezBuzzer buzzer(BUZZER_PIN);

// MOLE GAME
int score = 0;
unsigned long moleTimers[LED_COUNT];
unsigned long spawnTimer;
int spawnSpeedMs = 1500;
float spawnSpeedChange = .9;
int moleEscapeMs = 3500;
float escapeSpeedChange = .9;
int roundNum = 1;
int hits = 0;
int lives = 5;

bool mole_is_running(int idx) {
  return !mole_is_stopped(idx) && !mole_has_escaped(idx);
}

bool mole_is_stopped(int idx) {
  return moleTimers[idx] == 0;
}

bool mole_has_escaped(int idx) {
  return !mole_is_stopped(idx) && (millis() - moleTimers[idx]) > moleEscapeMs;

}

void renderMoles(){
  // using interpolation color moles based on how close they are to expiring
  for (int i=0; i<LED_COUNT; i++) {
    if (mole_is_running(i)) {
      leds[i] = CHSV(lerp8by8(120, 0, (millis() - moleTimers[i])/moleEscapeMs), 255, 255);
    } else {
      leds[i] = CHSV(0, 0,0);
    }
  }
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
  int randInt = random(openPositions.size());
  Serial.print("Spawning a mole at pos: ");
  Serial.println(openPositions.at(randInt));
  moleTimers[openPositions.at(randInt)] = millis();
}

void resetMole(int idx){
  // stop the mole's timer
  moleTimers[idx] = 0;
}

void checkForMoleHits(){
  // Get key and see if it matches an active mole
  char key = keypad.getKey();
  if (key == NO_KEY) {return;}
  Serial.print(key);
  bool status = mole_is_running((int)key);
  Serial.print(" running status is: ");
  Serial.println(status);
  if (status) {
    Serial.println("HIT! ");
    hits++;
    score++;
    resetMole((int)key);
  }
}

void checkForMoleEscapes(){
  // check to see if mole's timer timed out
  // if so decrement lives
  // then reset mole
  for (int i=0; i<LED_COUNT; i++) {
    if (mole_has_escaped(i)) {
      Serial.print(i);
      Serial.println(" HAS ESCAPED!");
      lives--;
      Serial.print("Lives left: ");
      Serial.println(lives);
      resetMole(i);
    }
  }}

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

int run() {
  // turn off all leds
  FastLED.clear();
  FastLED.show();

  // start loop, while lives > 0
  spawnTimer = millis();
  while (true) {
    while (hits < 10) {
      checkForMoleEscapes();
      if (lives <= 0) {
        Serial.println("GAME OVER!");
        Serial.print("Your score: ");
        Serial.println(score);
        return score;
      }
      if (millis() - spawnTimer > spawnSpeedMs) {
        spawnRandomMole();
        spawnTimer = millis();
      }
      checkForMoleHits();
      renderMoles();
     } 
    spawnSpeedMs = (int)(spawnSpeedMs * .8);
    moleEscapeMs = (int)(moleEscapeMs * .9);
    roundNum++;
  }
}

 // END MOLE GAME

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  delay(3000); // power on safety delay

  // LED SETUP
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // OLED SETUP
  // display.display(); // shows adafruit splash
  // delay(1500);
  // display.clearDisplay();

  // BUZZER SETUP
  // pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Serial.println(keypad.getKey());
  // buzzer.loop(); // MUST call the buzzer.loop() function in loop()
  Serial.println("Starting Mole!");
  moleGameReset();
  run();
}
