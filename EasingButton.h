#ifndef EasingButton_h
#define EasingButton_h

#include "MilliClock.h"

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

const int EASING_OFF = 0; // Easing button is completely off.
const int EASING_NO_CHANGE = 1; // Easing button says no change.
const int EASING_SLOW_CHANGE = 2; // Easing button says low change value
const int EASING_FAST_CHANGE = 3; // Easing button says fast change value

const unsigned long EASING_INTERVAL = 150;
const unsigned long EASING_FASTSWITCH_THRESHOLD = 3000; // Fast switch at 3s


class EasingButton {

    private:
        MilliClock* rtc;

        int inputPin;
        int reading;
        int previousButton = HIGH;
        int switchState;
        int buttonState;

        unsigned long previous;
        unsigned long held;
        int longChange;

    public:
        // Pin to read from.
        // Held at pull up high.  lChange represents the
        // value to return for long held depresses.
        EasingButton(MilliClock* rtc, int inputPin, int longChange);

        // Get the current switch state
        // This is not the same as the button State.
        int getSwitchState();

        // Get the current button state.
        // This is not the same as the switch State.
        int getButtonState();

        // Return a numerical change representing
        // the amount to change a value for the given button
        int getChange();

        // Call on each cycle to update internal state.
        void update();
};

#endif // EasingButton.h