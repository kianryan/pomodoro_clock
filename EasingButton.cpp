#include "EasingButton.h"

EasingButton::EasingButton(int pin, int lChange) {
    inputPin = pin;
    longChange = lChange;

    pinMode(inputPin, INPUT);
    digitalWrite(inputPin, HIGH);
}

int EasingButton::getState() {
    return switchstate;
}

int EasingButton::getChange() {

    int changeValue;

    switch (switchstate) {
        case HIGH:
            changeValue = 0;
            break;
        case LOW:
            changeValue = 1;
            break;
        case REALLY_LOW:
            changeValue = longChange;
            break;
    }

    return changeValue;
}

void EasingButton::update() {
    reading = digitalRead(inputPin);
    switchstate = reading;

    // If the switch has changed to low.
    if ((reading != previous) && (reading == LOW)) {
        time = millis();
    }

    // If the reading is *still* LOW and time has elapsed.
    if ((reading == LOW) && ((millis() - time) > fastswitch)) {
        // whatever the switch is at, its been there for a long time
        // so lets settle on it!
        switchstate = REALLY_LOW;
    }

    previous = reading;
}
