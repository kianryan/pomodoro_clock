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
        PomoTimer(int declaredResetValue);

        void setClock(RTC_DS1307* clock);

        void start();

        void stop();

        // If the timer is within the boundaries
        // and still running, returns the current
        // time remaining.  Otherwise, returns -1.
        TimeSpan time();
};

#endif // PomoTimer.h