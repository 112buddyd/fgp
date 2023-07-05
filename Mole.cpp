#include "Mole.h"

Mole::Mole(Keypad keypad, CRGB leds, ezBuzzer buzzer, Adafruit_SSD1306 display, byte size) {
  this->keypad = keypad; 
  this->keypad = keypad; 
  this->CRGB = CRGB; 
  this->ezBuzzer = ezBuzzer; 
  this->display = display;
  this->size = size;
  int score = 0;
  bool moles[size]; // might be able to do away with this in favor of moleTimers
  millisDelay moleTimers[size];
  millisDelay spawnTimer;
  int spawnSpeedMs = 3000;
  float spawnSpeedChange = .8;
  int moleEscapeMs = 3000;
  float escapeSpeedChange = .9;
  int roundNum = 1;
  int hits = 0;
  int lives = 3;
}

void Mole::renderMoles(){
  // using interpolation color moles based on how close they
  // are to expiring
}

void Mole::spawnRandomMole(){
  // randomly choose to spawn new mole in empty hole
  // set moles value to true
  // start mole timer
}

void Mole::resetMole(){
  // stop/reset the mole's timer
  // set the mole in moles to false
}

void Mole::checkForMoleHits(){
  // check to see if any active mole has a
  // matching button press. if so, add to score and reset mole
}
void Mole::checkForMoleEscapes(){
  // check to see if mole's timer timed out
  // if so decrement lives
  // then reset mole
}

int Mole::run() {
  // turn off all leds
  
  // show animation for game start

  // start loop, while lives > 0
  spawnTimer.start();
  loop {
    while (hits < 10) {
      if (spawnTimer.justFinished()) {
        spawn_random_mole();
        spawnTimer.start(spawnSpeedMs);
      }
      checkForMoleHits();
      checkForMoleEscapes();
      if (lives <= 0) {
        return score;
      }
      renderMoles();
     } 
   spawnSpeedMs = (int)(spawnSpeedMs * .8);
   moleEscapeMs = (int)(moleEscapeMs * .9);
   roundNum++;
  }
}
