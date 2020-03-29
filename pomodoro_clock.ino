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
   timer.start();
}

void loop() {

    tiltSwitch.update();
    buttonManager.update();

    // check the tilt state.
    int newDirection = tiltSwitch.getState();
    if (direction != newDirection) {
        direction = newDirection;
        timer.reset(timers[direction]);
        timer.start();
    }

    // check the button state.
    int state = buttonManager.getState();

    switch (state) {
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

    delay(250);

}

