#include "PomoTimer.h"
#include "RTClib.h"

PomoTimer::PomoTimer(RTC_DS1307* clock) {
    rtc = clock;
}

void PomoTimer::reset(int declaredResetValue) {
    stop();
    resetValue = declaredResetValue;

    remaining = TimeSpan(resetValue);
    time();
}

void PomoTimer::start() {
    running = true;
    previous = rtc->now();
    time();
}

void PomoTimer::stop() {
    running = false;
}

void PomoTimer::startStop() {
    running = !running;
}

void PomoTimer::changeTime(int seconds) {
    remaining = remaining + TimeSpan(seconds);
}

// If the timer is within the boundaries
// and still running, returns the current
// time remaining.  Otherwise, returns -1.
TimeSpan PomoTimer::time() {
    if (running)
    {
        DateTime current = rtc->now();

        // If still running, determine if we
        // should still be running.
        TimeSpan interval = current - previous;
        remaining = remaining - interval;
        if (remaining.totalseconds() <= 0) {
            running = false;
            remaining = TimeSpan(0);
        } else {
            previous = current;
        }
    }
    return remaining;
}