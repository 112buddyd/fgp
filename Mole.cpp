#include "Mole.h"
 
Mole::Mole(Keypad keypad, CRGB leds[9], ezBuzzer buzzer, Adafruit_SSD1306 display) {
  this->keypad = keypad; 
  this->leds = leds; 
  this->buzzer = buzzer; 
  this->display = display;
  int score = 0;
  millisDelay moleTimers[9];
  millisDelay spawnTimer;
  int spawnSpeedMs = 2000;
  float spawnSpeedChange = .9;
  int moleEscapeMs = 3000;
  float escapeSpeedChange = .9;
  int roundNum = 1;
  int hits = 0;
  int lives = 3;
}

void Mole::renderMoles(){
  // using interpolation color moles based on how close they are to expiring
  for (int i=0; i<9; i++) {
    if (moleTimers[i].isRunning()) {
      leds[i] = CHSV(lerp8by8(60, 0, moleTimers[i].remaining()/moleEscapeMs), 255, 255);
    }
  }
  FastLED.show();
}

void Mole::spawnRandomMole(){
  // randomly choose to spawn new mole in empty hole and start timer
  Vector<int> openPositions;
  for (int i=0; i<9; i++) {
    if (!moleTimers[i].isRunning()) {
      openPositions.push_back(i);
    }
  }
  int randInt = random(openPositions.size());
  moleTimers[randInt].start(moleEscapeMs);
}

void Mole::resetMole(int idx){
  // stop the mole's timer
  moleTimers[idx].stop();
}

void Mole::checkForMoleHits(){
  // Get key and see if it matches an active mole
  char key = keypad.getKey();
  if (key == NO_KEY) {return;}
  if (moleTimers[(int)key].isRunning()) {
    hits++;
    resetMole((int)key);
  }
}

void Mole::checkForMoleEscapes(){
  // check to see if mole's timer timed out
  // if so decrement lives
  // then reset mole
  Vector<int> justEscaped;
  for (int i=0; i<9; i++) {
    if (moleTimers[i].justFinished()) {
      justEscaped.push_back(i);
      resetMole(i);
    }
  }
  lives -= justEscaped.size();
}

int Mole::run() {
  // turn off all leds
  FastLED.clear();
  FastLED.show();

  // start loop, while lives > 0
  spawnTimer.start(spawnSpeedMs);
  while (true) {
    while (hits < 10) {
      checkForMoleEscapes();
      if (lives <= 0) {
        return score;
      }
      if (spawnTimer.justFinished()) {
        spawnRandomMole();
        spawnTimer.start(spawnSpeedMs);
      }
      checkForMoleHits();
      renderMoles();
     } 
    spawnSpeedMs = (int)(spawnSpeedMs * .8);
    moleEscapeMs = (int)(moleEscapeMs * .9);
    roundNum++;
  }
}
