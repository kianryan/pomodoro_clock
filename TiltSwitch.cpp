#include "TiltSwitch.h"

TiltSwitch::TiltSwitch(int switchPin)
    : switchPin { switchPin } {

  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);   // turn on the built in pull-up resistor
  switchstate = digitalRead(switchPin); // grab an initial state
}

int TiltSwitch::getState() {
    return switchstate;
}

void TiltSwitch::update() {
    reading = digitalRead(switchPin);

    // If the switch changed, due to bounce or pressing...
    if (reading != previous) {
        // reset the debouncing timer
        time = millis();
    }

    if ((millis() - time) > debounce) {
        // whatever the switch is at, its been there for a long time
        // so lets settle on it!
        switchstate = reading;
    }

    // Save the last reading so we keep a running tally
    previous = reading;
}
