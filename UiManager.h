#ifndef UiManager_h
#define UiManager_h

#include "LedControl.h"

class UiManager {

    private:
        LedControl* lc;


    public:
        UiManager(int dataPin, int clkPin, int csPin);

        ~UiManager();

        void startup();

        void clearDisplay();

        void display(int totalSeconds);
};

#endif // UiManager.h