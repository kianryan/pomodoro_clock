#include "UiManager.h"



UiManager::UiManager(MilliClock* rtc, int dataPin, int clkPin, int csPin, int piezoPin)
     : rtc{rtc}, piezoPin{piezoPin} {
     lc = new LedControl(dataPin, clkPin, csPin, 1);
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
   lc->setDigit(0, 2, 8, false);
   lc->setDigit(0, 3, 8, false);
}

void UiManager::clearDisplay() {
    lc->clearDisplay(0);
}

void UiManager::display(unsigned long totalSeconds, bool direction) {

    /* In here, we only have seconds...*/

    byte digits[] = {0, 0, 0, 0};

    unsigned long mins = totalSeconds / 60;
    unsigned long secs = totalSeconds - (mins * 60);

    digits[0] = (mins / 10) % 10;
    digits[1] = mins % 10;

    // if (direction) {
    //     lc->setDigit(0, 0, (byte)tens, false);
    //     lc->setDigit(0, 1, (byte)ones, true);
    // } else {
    //     lc->setInverseDigit(0, 3, (byte)tens, false);
    //     lc->setInverseDigit(0, 2, (byte)ones, true);
    // }

    digits[2] = (secs / 10) % 10;
    digits[3] = secs % 10;

    if (direction) {
        lc->setDigit(0, 0, digits[0], false);
        lc->setDigit(0, 1, digits[1], true);
        lc->setInverseDigit(0, 2, digits[2], false);
        lc->setInverseDigit(0, 3, digits[3], false);
    } else {
        lc->setDigit(0, 3, digits[0], false);
        lc->setDigit(0, 2, digits[1], true);
        lc->setInverseDigit(0, 1, digits[2], false);
        lc->setInverseDigit(0, 0, digits[3], false);
    }
}

void UiManager::silenceAlarm() {
    isSilenced = true;
    noTone(piezoPin);
}

void UiManager::resetAlarm() {
    isSilenced = false;
    toggleTone = false;
}

void UiManager::alarm() {

    if (!isSilenced) {
        unsigned long current = rtc-> now();
        if (current - lastTone >= ALARM_INTERVAL) {
            lastTone = current;
            toggleTone = !toggleTone;
        }

        if (toggleTone) {
            tone(piezoPin,65); // G2
        } else {
            noTone(piezoPin);
        }
    }
}
