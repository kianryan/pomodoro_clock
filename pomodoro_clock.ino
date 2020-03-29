#include "LedControl.h"
#include "RTClib.h"
#include "PomoTimer.h"
#include "TiltSwitch.h"
#include "EasingButton.h"
#include "ButtonManager.h"
#include "UiManager.h"

RTC_DS1307 rtc;
// LedControl lc = LedControl(12, 11, 10, 1);

int timers[] = {300, 100};

PomoTimer timer(&rtc);
TiltSwitch tiltSwitch(2);
ButtonManager buttonManager(3, 4, 5);
UiManager uiManager(12, 11, 10);

int direction;

bool preflight;
DateTime preflightTime;
bool preflightDisplayState;


void setup() {

   // Serial for debugging
   Serial.begin(9600);

   // Setup the clock
   rtc.begin();
   rtc.adjust(DateTime(__DATE__, __TIME__));

   uiManager.startup();
   delay(1000);

   direction = tiltSwitch.getState(); // Get an inital direction.
   timer.reset(timers[direction]);
   preflight = true;
   preflightTime = rtc.now();
}

void loop() {

    tiltSwitch.update();
    buttonManager.update();

    // On a change in tilt state,
    int newDirection = tiltSwitch.getState();
    if (direction != newDirection) {
        direction = newDirection;
        timer.reset(timers[direction]);

        preflight = true;
        preflightTime = rtc.now();
    }

    if (preflight) {
        int interval = (rtc.now() - preflightTime).totalseconds();
        Serial.print("Preflight time:");
        Serial.println(interval);

        /* We're only in pre-flight for ~5 seconds. */
        if (interval > 3) {
            preflight = false;
            timer.start();
            // Persist the new time to ROM.
        }

        // check the button state.
        switch (buttonManager.getState()) {
           case CHANGE_TIMER:
                int change = buttonManager.getChange();
                timers[direction] += change;
                timer.changeTime(change);
                break;
        }


        if (preflightDisplayState) {
            Serial.println(timer.time().totalseconds());
            uiManager.display(timer.time().totalseconds());
        } else {
            uiManager.clearDisplay();
        }
        preflightDisplayState = ! preflightDisplayState;
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
        if (totalseconds >= 0) {
            uiManager.display(totalseconds);
        }
    }

    delay(250);

}

