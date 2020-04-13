#include "EasingButton.h"


EasingButton::EasingButton(MilliClock* rtc, int inputPin, int longChange)
    : rtc{rtc}, inputPin{inputPin}, longChange{longChange}  {

    pinMode(inputPin, INPUT);
    digitalWrite(inputPin, HIGH);
}

int EasingButton::getSwitchState() {
    return switchState;
}

int EasingButton::getButtonState() {
    return buttonState;
}

int EasingButton::getChange()
{

    int changeValue;

    switch (buttonState)
    {
        case EASING_OFF:
        case EASING_NO_CHANGE:
            changeValue = 0;
            break;
        case EASING_SLOW_CHANGE:
            changeValue = 1;
            break;
        case EASING_FAST_CHANGE:
            changeValue = longChange;
            break;
    }

    return changeValue;
}

void EasingButton::update() {
    reading = digitalRead(inputPin);
    switchState = reading;

    unsigned long current = rtc->now();
    unsigned long interval = current - previous;

    if (switchState == HIGH) {
        buttonState = EASING_OFF;
        previous = current;

        held = 0;
    } else {
        if (held > EASING_FASTSWITCH_THRESHOLD) {
            previous = current;
            buttonState = EASING_FAST_CHANGE;
        } else {
            if (interval > EASING_INTERVAL) {
                previous = current;
                buttonState = EASING_SLOW_CHANGE;

                held += interval;
            } else {
                buttonState = EASING_NO_CHANGE;
            }
        }
   }
}
