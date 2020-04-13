#include "PomoTimer.h"

PomoTimer::PomoTimer(MilliClock* rtc):
    rtc { rtc } {
}

void PomoTimer::reset(int declaredResetValue) {
    stop();
    resetValue = declaredResetValue;

    remaining = (unsigned long)resetValue * 1000;
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
    remaining = remaining + (seconds * 1000);
}

// If the timer is within the boundaries
// and still running, returns the current
// time remaining.  Otherwise, returns -1.
TimeSpan PomoTimer::time() {
    if (running)
    {
        unsigned long current = rtc->now();

        // If still running, determine if we
        // should still be running.
        unsigned long interval = current - previous;
        if (interval >= remaining) {
            running = false;
            remaining = 0;
        } else {
            remaining = remaining - interval;
            previous = current;
        }
    }
    return TimeSpan(remaining);
}