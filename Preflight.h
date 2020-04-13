#include "PomoTimer.h"
#include "MilliClock.h"

#ifndef Preflight_h
#define Preflight_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

const int PREFLIGHT_RUNTIME = 3;          // PREFLIGHT RUNTIME, SECONDS.
const int PREFLIGHT_BLINK_INTERVAL = 300; // PREFLIGHT BLINK TIME, MILLISECONDS.

class Preflight {

    private:
        MilliClock* rtc;
        PomoTimer* timer;
        bool running;

        unsigned long remaining;
        unsigned long previous;

        unsigned long lastFlip;
        bool displayOn;
    public:

        Preflight(MilliClock* rtc, PomoTimer* timer);

        // Starts the preflight from a stopped state.
        // Has no effect if already started.
        void start(int declaredResetValue);

        // Change the preflight timer by the given number of milliseconds.
        void changeTime(int milliseconds);

        // If the timer is within the boundaries
        // and still running, returns the current
        // time remaining.

        TimeSpan time();

        // Returns if pre-flight is currently running.
        bool isRunning();

        // Return if display should currently be on or not.
        bool isDisplayOn();
};

#endif // Preflight_h