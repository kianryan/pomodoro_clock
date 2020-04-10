#include "LedControl.h"
#include "RTClib.h"
#include "PomoTimer.h"
#include "TiltSwitch.h"
#include "EasingButton.h"
#include "ButtonManager.h"
#include "UiManager.h"

RTC_Millis rtc;
// LedControl lc = LedControl(12, 11, 10, 1);

int timers[] = {300, 10};

PomoTimer timer(&rtc);
TiltSwitch tiltSwitch(2);
ButtonManager buttonManager(3, 4, 5);
UiManager uiManager(10, 11, 12, A3);

int direction;

bool preflight;
DateTime preflightTime;
bool preflightDisplayState;

bool alarmSilenced;


void setup() {

   // Serial for debugging
   Serial.begin(9600);

   // Setup the clock
   rtc.begin(DateTime(F(__DATE__), F(__TIME__)));

   uiManager.startup();
   delay(1000);

   direction = tiltSwitch.getState(); // Get an inital direction.
   startPreFlight();
}

void loop() {

    tiltSwitch.update();
    buttonManager.update();

    // On a change in tilt state,
    int newDirection = tiltSwitch.getState();
    if (direction != newDirection) {
        direction = newDirection;

        startPreFlight();
    }

    if (preflight) {

        /*
         * If in pre-flight, allow the user to adjust the global reset
         * time, before time timer starts.  The user has three seconds to
         * to this before the user timer starts.
        */

        int interval = (rtc.now() - preflightTime).totalseconds();
        Serial.print("Preflight time:");
        Serial.println(interval);

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
            uiManager.display(timer.time().totalseconds(), direction);
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

        uiManager.display(totalseconds, direction);

        Serial.print("Seconds remaining: ");
        Serial.println(totalseconds);

        if (totalseconds <= (int32_t)0) {
            Serial.println("SOUND ALARM");

            /* Replace this with any buttons? */
            if (buttonManager.getState() != NO_BUTTON) {
                alarmSilenced = true;
            }

            if (!alarmSilenced) {
                uiManager.alarm();
            }
        }
   }

    delay(250);
}

void startPreFlight() {
    timer.reset(timers[direction]);
    preflight = true;
    preflightTime = rtc.now();
    alarmSilenced = false;
}

