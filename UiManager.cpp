#include "UiManager.h"

UiManager::UiManager(int dataPin, int clkPin, int csPin) {
     lc = new LedControl(dataPin, clkPin, csPin);
}

UiManager::~UiManager() {
    delete lc;
}

void UiManager::startup() {
   //wake up the MAX72XX from power-saving mode
   lc->shutdown(0,false);
   //set a medium brightness for the Leds
   lc->setIntensity(0,10);

   // debug
   lc->setDigit(0, 0, 8, true);
   lc->setDigit(0, 1, 8, true);
   lc->setDigit(0, 2, 8, true);
   lc->setDigit(0, 3, 8, true);
}

void UiManager::clearDisplay() {
    lc->clearDisplay(0);
}

void UiManager::display(int totalSeconds) {

    /* In here, we only have seconds...*/

    int mins = totalSeconds / 60;
    int secs = totalSeconds - (mins * 60);

    int ones = mins % 10;
    mins = mins/10;

    int tens = mins % 10;
    lc->setDigit(0, 0, (byte)tens, false);
    lc->setDigit(0, 1, (byte)ones, true);

    ones = secs % 10;
    secs = secs/10;
    tens = secs % 10;

    lc->setDigit(0, 2, (byte)tens, false);
    lc->setDigit(0, 3, (byte)ones, false);
}