#ifndef UiManager_h
#define UiManager_h

#include "LedControl.h"

class UiManager {

    private:
        LedControl* lc;
        int piezoPin;
        bool toggleTone;
    public:
        UiManager(int dataPin, int clkPin, int csPin, int piezoPin);

        ~UiManager();

        void startup();

        void clearDisplay();

        void display(unsigned long totalSeconds, int direction);

        void alarm();
};

#endif // UiManager.h