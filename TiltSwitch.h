#ifndef TiltSwitch_h
#define TiltSwitch_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class TiltSwitch {

    private:
        int switchPin;
        int reading;
        int previous = LOW;
        int switchstate;

        // the following variables are long because the time, measured in miliseconds,
        // will quickly become a bigger number than can be stored in an int.
        long time = 0;         // the last time the output pin was toggled
        long debounce = 1000;   // the debounce time, increase if the output flickers



    public:
        TiltSwitch(int pin);

        // Return if the tilt sensor
        // is currently pointing up.
        int getState();

        // Update state of tilt switch.
        void update();
};

#endif // TiltSwitch.h