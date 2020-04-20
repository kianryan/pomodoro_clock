#include "LedControl.h"
#include "MilliClock.h"
#include "PomoTimer.h"
#include "TiltSwitch.h"
#include "EasingButton.h"
#include "ButtonManager.h"
#include "UiManager.h"
#include "Preflight.h"

MilliClock rtc;

int timers[] = {100, 10};

PomoTimer timer(&rtc);
TiltSwitch tiltSwitch(2);
ButtonManager buttonManager(&rtc, 3, 4, 5);
UiManager uiManager(10, 11, 12, A3);
Preflight preflight(&rtc, &timer);

int direction;

bool alarmSilenced;


void setup() {

   // Serial for debugging
   Serial.begin(9600);


   uiManager.startup();
   delay(1000);

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
                int change = buttonManager.getChange();

                timers[direction] = timer.changeTime(change);
                pause = true;
                break;
        }

       TimeSpan interval = preflight.time(pause); // get preflight time.
       int32_t totalseconds = interval.totalseconds();

        if (preflight.isDisplayOn()) {
            uiManager.display(timer.time().totalseconds(), direction);
        } else {
            uiManager.clearDisplay();
        }
    } else {

        // check the button state.
        switch (buttonManager.getState()) {
            case NO_BUTTON:
                // Nothing to do here.
                break;
            case TOGGLE_TIMER:
                timer.startStop();
                break;
            case CHANGE_TIMER:
                timer.changeTime(buttonManager.getChange());
                break;
        }

        // Get the time from the timer and display.
        TimeSpan interval = timer.time();
        int32_t totalseconds = interval.totalseconds();

        uiManager.display(totalseconds, direction);

        Serial.print("Seconds remaining: ");
        Serial.println(totalseconds);

        /* Need to handle the cancelling code better. */
        // if (totalseconds <= (int32_t)0) {
        //     Serial.println("SOUND ALARM");

        //     /* Replace this with any buttons? */
        //     if (buttonManager.getState() != NO_BUTTON) {
        //         alarmSilenced = true;
        //     }

        //     if (!alarmSilenced) {
        //         uiManager.alarm();
        //     }
        // }
   }
}
