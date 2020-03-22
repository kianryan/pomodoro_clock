#include "PomoTimer.h"
#include "RTClib.h"

PomoTimer::PomoTimer(RTC_DS1307* clock) {
    rtc = clock;
}

void PomoTimer::start(int declaredResetValue) {
    resetValue = declaredResetValue;

    running = true;
    startedTime = rtc->now();
    Serial.print("Started timing at:");
    Serial.println(startedTime.unixtime());
}

void PomoTimer::stop() {
    running = false;
    Serial.print("Stopped timing.");
}

// If the timer is within the boundaries
// and still running, returns the current
// time remaining.  Otherwise, returns -1.
TimeSpan PomoTimer::time() {
    if (running)
    {
        // If still running, determine if we
        // should still be running.
        TimeSpan interval = rtc->now() - startedTime;
        if (interval.totalseconds() > resetValue) {
            running = false;
        } else {
            return TimeSpan(resetValue) - interval;
        }
    }

    return TimeSpan(0);
}