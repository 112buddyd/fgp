// Mole.h
#ifndef Mole_h
#define Mole_h

#include <Arduino.h>
#include <Vector.h>
#include "Hardware.h"

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
        Hardware hw;

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
        Mole(Hardware hw);
        void run();
};
#endif