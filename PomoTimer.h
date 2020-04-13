#ifndef PomoTimer_h
#define PomoTimer_h

#include "MilliClock.h"

class PomoTimer {
    private:
        MilliClock* rtc;
        int resetValue;
        bool running;

        unsigned long remaining;
        unsigned long previous;
    public:
        PomoTimer(MilliClock* clock);

        // Start tht timer counting down to the
        // reset resetValue.
        void reset(int declaredResetValue);

        // Start the clock from a stopped state.
        // Has no effect if already started.
        void start();

        // Stop the clock before the timer has
        // been reached.
        void stop();

        // Toggle the current running state.
        void startStop();

        // Change the timer by the given number of seconds.
        void changeTime(int changeSeconds);

        // If the timer is within the boundaries
        // and still running, returns the current
        // time remaining.
       TimeSpan time();
};

#endif // PomoTimer.h