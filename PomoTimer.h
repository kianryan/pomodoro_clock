#ifndef PomoTimer_h
#define PomoTimer_h

#include "RTClib.h"

class PomoTimer {
    private:
        int resetValue;
        DateTime startedTime;
        bool running;
        RTC_DS1307* rtc;

    public:
        PomoTimer(RTC_DS1307* clock);

        // Start tht timer counting down to the
        // reset resetValue.
        void start(int declaredResetValue);

        // Stop the clock before the timer has
        // been reached.
        void stop();

        // Update the reset value to the declaredResetValue.
        void updateResetTimer(int declaredResetValue);

        // If the timer is within the boundaries
        // and still running, returns the current
        // time remaining.  Otherwise, returns -1.
        TimeSpan time();
};

#endif // PomoTimer.h