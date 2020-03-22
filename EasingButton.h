#ifndef EasingButton_h
#define EasingButton_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define REALLY_LOW 2

// An easing button should remain "sticky" for a number
// of cycles, before increasing until released.
// We can measure in one of three states,
// LOW, HIGH, REALLY_HIGH (2).

// We're going to base this off the debounce code we
// used for the tilt sensor.


class EasingButton {

    private:
        int inputPin;
        int reading;
        int previous = HIGH;
        int switchstate;

        long time;
        long fastswitch = 5000;

        int longChange = 5;

    public:
        // Pin to read from.
        // Held at pull up high.  lChange represents the
        // value to return for long held depresses.
        EasingButton(int pin, int lChange);

        // Get the current switch state
        // HIGH (off), LOW (on), REALLY_LOW (long held)
        int getState();

        // Return a numerical change representing
        // the amount to change a value for the given button
        int getChange();

        // Call on each cycle to update internal state.
        void update();
};

#endif // EasingButton.h