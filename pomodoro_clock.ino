#include "LedControl.h"
#include "MilliClock.h"
#include "PomoTimer.h"
#include "TiltSwitch.h"
#include "EasingButton.h"
#include "ButtonManager.h"
#include "UiManager.h"
#include "Preflight.h"
#include <EEPROM.h>

MilliClock rtc;

int timers[] = {0, 0};

PomoTimer timer(&rtc);
TiltSwitch tiltSwitch(2);
ButtonManager buttonManager(&rtc, 3, 4, 5);
UiManager uiManager(&rtc, 10, 11, 12, A3);
Preflight preflight(&rtc, &timer);

int direction;

bool alarmSilenced;


void setup() {

   // Serial for debugging
//    Serial.begin(9600);

   uiManager.startup();
   delay(1000);

   /* Initialize EEPROM, or timers from EEPROM */
   byte init = EEPROM.read(0);
   if (init == 255) {
       timers[0] = 1200;
       timers[1] = 300;
       EEPROM.put(1, timers[0]);
       EEPROM.put(sizeof(int) + 1, timers[1]);
       EEPROM.put(0, 0);
   } else {
       EEPROM.get(1, timers[0]);
       EEPROM.get(sizeof(int) + 1, timers[1]);
   }

   direction = tiltSwitch.getState(); // Get an inital direction.
   preflight.start(timers[direction]);
}

void loop() {

    tiltSwitch.update();
    buttonManager.update();

    // On a change in tilt state,
    int newDirection = tiltSwitch.getState();
    if (direction != newDirection) {
        direction = newDirection;

        preflight.start(timers[direction]);
    }

    if (preflight.isRunning()) {

        /* Take button input, if button press, increase duration
           of preflight, and increase time displayed on clock. */

        // check the button state.
        bool pause = false;
        switch (buttonManager.getState()) {
           case CHANGE_TIMER:
                int change = buttonManager.getChange(direction);

                timers[direction] = timer.changeTime(change);
                pause = true;
                break;
        }

       TimeSpan interval = preflight.time(pause); // get preflight time.
       if (interval.milliseconds() == 0) {        // update EEPROM with last set time.
           EEPROM.put((direction * sizeof(int)) + 1, timers[direction]);
           uiManager.resetAlarm();
       }

        if (preflight.isDisplayOn()) {
            uiManager.display(timer.time().totalseconds(), direction);
        } else {
            uiManager.clearDisplay();
        }
    } else {

        // check the button state.
        int buttonState = buttonManager.getState();
        switch (buttonState) {
            case NO_BUTTON:
                // Nothing to do here.
                break;
            case TOGGLE_TIMER:
                timer.startStop();
                break;
            case CHANGE_TIMER:
                timer.changeTime(buttonManager.getChange(direction));
                break;
        }

        // Get the time from the timer and display.
        TimeSpan interval = timer.time();
        unsigned long millis = interval.milliseconds();

        // If the timer is not currently running,
        // and there's time on the clock, start the timer.
        if (millis > 0 && ! timer.isRunning()) {
            uiManager.resetAlarm();
            timer.start();
        }
        // If there's no time on the clock, alarm.
        // If the button is depressed, silence.
        else if (millis == 0) {
            if (buttonState != NO_BUTTON) {
                uiManager.silenceAlarm();
            }
            uiManager.alarm();
        }

        uiManager.display(interval.totalseconds(), direction);
   }
}
