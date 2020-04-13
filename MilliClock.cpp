#include "MilliClock.h"

unsigned long MilliClock::now() {
    return millis(); // we may handle rollover later.
}

TimeSpan::TimeSpan(unsigned long _milliseconds):
    _milliseconds(_milliseconds) {
}

unsigned long TimeSpan::totalseconds() {
    return _milliseconds / 1000;
}

