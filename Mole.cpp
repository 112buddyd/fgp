// Mole.cpp
#include "Mole.h"

Mole::Mole(Hardware hw) {
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
  hw = hw;

}
// MOLE GAME

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
      hw.leds[i].red = hw.colorOne.red * (moleEscapeMs - delta) / moleEscapeMs + hw.colorTwo.red * delta/ moleEscapeMs;
      hw.leds[i].green = hw.colorOne.green * (moleEscapeMs - delta) / moleEscapeMs + hw.colorTwo.green * delta/ moleEscapeMs;
    } else {
      hw.leds[i] = CRGB(0, 0, 0);
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
  char ckey = hw.keypad.getKey();
  if (ckey == NO_KEY) {return;}
  int key  = hw.key_to_led(ckey);
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
  hw.display.clearDisplay();
  hw.display.setCursor(0,0);
  hw.display.println("WHACKAMOLE");
  hw.display.println(" GO GO GO ");
  hw.display.display();
  fill_solid(hw.leds, LED_COUNT, CRGB(255, 0, 0));
  FastLED.show();
  delay(750);
  fill_solid(hw.leds, LED_COUNT, CRGB(255, 255, 0));
  FastLED.show();
  delay(750);
  fill_solid(hw.leds, LED_COUNT, CRGB(0, 255, 0));
  FastLED.show();
  delay(1250);
}

void Mole::endAnimation() {
  int i = 0;
  hw.display.clearDisplay();
  hw.display.setCursor(0,0);
  hw.display.println("GAME OVER!");
  hw.display.print("Score: ");
  hw.display.println(score);
  hw.display.display();

  fill_solid(hw.leds, LED_COUNT, CRGB(255, 0, 0));
  FastLED.show();
  delay(500);
  fill_solid(hw.leds, LED_COUNT, CRGB(0, 0, 0));
  FastLED.show();
  delay(250);
  fill_solid(hw.leds, LED_COUNT, CRGB(255, 0, 0));
  FastLED.show();
  delay(500);
  fill_solid(hw.leds, LED_COUNT, CRGB(0, 0, 0));
  FastLED.show();
  delay(250);
  fill_solid(hw.leds, LED_COUNT, CRGB(255, 0, 0));
  FastLED.show();
  delay(500);
  fill_solid(hw.leds, LED_COUNT, CRGB(0, 0, 0));
  FastLED.show();
  delay(250);
  
}

void Mole::run() {
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
  hw.display.clearDisplay();
  hw.display.setCursor(0,0);
  hw.display.print("Score: ");
  hw.display.println(score);
  hw.display.print("Lives: ");
  hw.display.println(lives);
  hw.display.display();                       
}

void Mole::drawNewGameScreen() {
  hw.display.clearDisplay();
  hw.display.setCursor(0,0);
  hw.display.println("WHACKAMOLE");
  hw.display.println("Hit a key!");
  hw.display.display();
}

// END MOLE GAME