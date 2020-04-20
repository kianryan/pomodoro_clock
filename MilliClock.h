#ifndef MilliClock_h
#define MilliClock_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// AdaFruit RTC Clock only allows for second level prevision.
// We need millisecond level prevision.
// So, define a clock that handles millisecond precision.

class MilliClock {

    public:

        // return the current time, in millis
        unsigned long now();
};

class TimeSpan {

    protected:
        unsigned long _milliseconds;

    public:
        TimeSpan(unsigned long milliseconds = 0);
        unsigned long totalseconds();
        unsigned long milliseconds();
};

#endif // MilliClock_h