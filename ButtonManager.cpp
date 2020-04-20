#include "ButtonManager.h"

ButtonManager::ButtonManager(MilliClock* rtc, int upPin, int downPin, int lChange) {
    buttons[0] = new EasingButton(rtc, upPin, lChange);
    buttons[1] = new EasingButton(rtc, downPin, lChange);
}

ButtonManager::~ButtonManager() {
    delete buttons[0];
    delete buttons[1];
}

void ButtonManager::update() {
    buttons[0]->update();
    buttons[1]->update();
}

int ButtonManager::getState() {

    int buttonState[2];
    buttonState[0] = buttons[0]->getSwitchState();
    buttonState[1] = buttons[1]->getSwitchState();

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

int ButtonManager::getChange(int direction) {

    int buttonState[2];
    buttonState[0] = buttons[0]->getSwitchState();
    buttonState[1] = buttons[1]->getSwitchState();

    /* When returning change, take direction into account */
    int flip = 1;
    if (direction == HIGH) {
        flip = -1;
    }

    if (buttons[0]->getSwitchState() != HIGH) return buttons[0]->getChange() * flip;
    if (buttons[1]->getSwitchState() != HIGH) return buttons[1]->getChange() * -1 * flip;

    return 0;
}