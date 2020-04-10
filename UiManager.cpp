#include "UiManager.h"

UiManager::UiManager(int dataPin, int clkPin, int csPin, int piezoPin)
     : piezoPin{piezoPin} {
     lc = new LedControl(dataPin, clkPin, csPin, piezoPin);
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

void UiManager::display(int totalSeconds, int direction) {

    /* In here, we only have seconds...*/

    int mins = totalSeconds / 60;
    int secs = totalSeconds - (mins * 60);

    int ones = mins % 10;
    mins = mins/10;

    int tens = mins % 10;

    if (direction == HIGH) {
        lc->setDigit(0, 0, (byte)tens, false);
        lc->setDigit(0, 1, (byte)ones, true);
    } else {
        lc->setInverseDigit(0, 3, (byte)tens, false);
        lc->setInverseDigit(0, 2, (byte)ones, true);
    }

    ones = secs % 10;
    secs = secs/10;
    tens = secs % 10;

    if (direction == HIGH) {
        lc->setDigit(0, 2, (byte)tens, false);
        lc->setDigit(0, 3, (byte)ones, false);
    } else {
        lc->setInverseDigit(0, 1, (byte)tens, false);
        lc->setInverseDigit(0, 0, (byte)ones, false);
    }
}

void UiManager::alarm() {

    /* Alarm will sound for three seconds, in 250ms bursts. */
    if (toggleTone) {
        tone(piezoPin, 262);
    }
    toggleTone = ! toggleTone;
}
