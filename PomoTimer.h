#ifndef PomoTimer_h
#define PomoTimer_h

#include "RTClib.h"

class PomoTimer {
    private:
        int resetValue;
        TimeSpan remaining;
        DateTime previous;
        bool running;
        RTC_DS1307* rtc;

    public:
        PomoTimer(RTC_DS1307* clock);

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
        // time remaining.  Otherwise, returns -1.
        TimeSpan time();
};

#endif // PomoTimer.h