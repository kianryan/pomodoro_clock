#ifndef ButtonManager_h
#define ButtonManager_h

#include "EasingButton.h"

const int NO_BUTTON = 0;
const int TOGGLE_TIMER = 1;
const int CHANGE_TIMER = 2;

class ButtonManager {

    private:

        EasingButton* upButton;
        EasingButton* downButton;
        bool toggleTimerFired;

    public:
        // Declare up button and down buttons
        ButtonManager(int upPin, int downPin, int lChange);

        // Destructor
        ~ButtonManager();

        // Update the current button state
        void update();

        // Return the current state
        int getState();

        // Return the current change
        int getChange();
};

#endif // ButtonManager.h