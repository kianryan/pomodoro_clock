#include "Preflight.h"

Preflight::Preflight(MilliClock* rtc, PomoTimer* timer)
    : rtc { rtc }, timer { timer } {
}

void Preflight::start(int declaredResetValue) {
    remaining = PREFLIGHT_RUNTIME * 1000; // sec to ms

    timer->reset(declaredResetValue); // Need access to timers
    running = true;
    previous = rtc->now();

    lastFlip = rtc->now(); // setup the display millis counter.
    displayOn = false;
}

TimeSpan Preflight::time(bool pause) {
    if (running)
    {
        unsigned long current = rtc->now();

        unsigned long interval = current - previous;

        if (interval >= remaining) {
            running = false;
            remaining = 0;

            timer->start();
        } else {
            if (!pause) {
                // If we're not pausing,
                // take the time off the interval.
                remaining -= interval;
            }
            previous = current;
        }
    }
    return TimeSpan(remaining);
}

bool Preflight::isRunning() {
    return running;
}

bool Preflight::isDisplayOn() {

    // Display should be cycled every PREFLIGHT_BLINK_INTERVAL millis.

    unsigned long current = rtc-> now();
    if (current - lastFlip >= PREFLIGHT_BLINK_INTERVAL) {
        lastFlip = current;
        displayOn = !displayOn;
    }

    return displayOn;
}