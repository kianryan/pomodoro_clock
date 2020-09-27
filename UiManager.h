#ifndef UiManager_h
#define UiManager_h

#include "MilliClock.h"
#include "LedControl.h"

const int ALARM_INTERVAL = 250;
const int ALARM_TONE = 262;

class UiManager {

    private:
        MilliClock* rtc;
        LedControl* lc;
        int piezoPin;

        unsigned long current;
        unsigned long lastTone;
        bool toggleTone;
        bool isSilenced;
    public:
        UiManager(MilliClock* rtc, int dataPin, int clkPin, int csPin, int piezoPin);

        ~UiManager();

        void startup();

        void clearDisplay();

        void display(unsigned long totalSeconds, bool direction);

        void alarm();

        void silenceAlarm();

        void resetAlarm();
};

#endif // UiManager.h