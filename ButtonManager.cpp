#include "ButtonManager.h"

ButtonManager::ButtonManager(MilliClock* rtc, int upPin, int downPin, int lChange) {
    upButton = new EasingButton(rtc, upPin, lChange);
    downButton = new EasingButton(rtc, downPin, lChange);
}

ButtonManager::~ButtonManager() {
    delete upButton;
    delete downButton;
}

void ButtonManager::update() {
    upButton->update();
    downButton->update();
}

int ButtonManager::getState() {

    int buttonState[2];
    buttonState[0] = upButton->getSwitchState();
    buttonState[1] = downButton->getSwitchState();

    if (buttonState[0] == HIGH && buttonState[1] == HIGH) {
        /* THIS IS A RESET STATE */
        toggleTimerFired = false;
        return NO_BUTTON;
    } else if (buttonState[0] != HIGH && buttonState[1] != HIGH &&
        ! toggleTimerFired) {

        // We will only fire this state once per depress
        toggleTimerFired = true;
        return TOGGLE_TIMER;
    } else if (buttonState[0] != buttonState[1]) {
        // We only allow this state to occur while one button is
        // depressed.
        return CHANGE_TIMER;
    }

    // If we can't determine state, return NO_BUTTON.
    return NO_BUTTON;
}

int ButtonManager::getChange() {

    int buttonState[2];
    buttonState[0] = upButton->getSwitchState();
    buttonState[1] = downButton->getSwitchState();

    if (upButton->getSwitchState() != HIGH) return upButton->getChange();
    if (downButton->getSwitchState() != HIGH) return downButton->getChange() * -1;

    return 0;
}