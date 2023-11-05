// Paint.h
#ifndef Paint_h
#define Paint_h

#include <Arduino.h>
#include "Hardware.h"

class Paint {
  private:
    Hardware hw;
    unsigned long start;
    CRGB getRandomColor();
    void changeColor(int idx);
    int evalGameOver();
    void drawNewGameScreen();
    void drawGameOverScreen(int score);

  public:
    Paint(Hardware hw);
    void run();
};

#endif