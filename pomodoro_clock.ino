#include "LedControl.h"
#include "RTClib.h"
#include "PomoTimer.h"
#include "TiltSwitch.h"
#include "EasingButton.h"

RTC_DS1307 rtc;
LedControl lc = LedControl(12, 11, 10, 1);

int timers[] = {300, 100};

PomoTimer timer(&rtc);
TiltSwitch tiltSwitch(2);
EasingButton increaseButton(3, 5);
EasingButton decreaseButton(4, 5);

int direction;

void setup() {
   //wake up the MAX72XX from power-saving mode
   lc.shutdown(0,false);
   //set a medium brightness for the Leds
   lc.setIntensity(0,10);

   // Serial for debugging
   Serial.begin(9600);

   // Setup the clock
   rtc.begin();
   rtc.adjust(DateTime(__DATE__, __TIME__));

   // debug
   lc.setDigit(0, 0, 8, true);
   lc.setDigit(0, 1, 8, true);
   lc.setDigit(0, 2, 8, true);
   lc.setDigit(0, 3, 8, true);

   delay(1000);

   direction = tiltSwitch.getState(); // Get an inital direction.
   timer.reset(timers[direction]);
   timer.start();
}

void loop() {

    increaseButton.update();
    decreaseButton.update();
    int buttonChange[2];
    buttonChange[0] = increaseButton.getChange();
    buttonChange[1] = decreaseButton.getChange();

    if (buttonChange[0] != 0 && buttonChange[1] != 0) {
        // Start/stop timer.
        timer.startStop();
    } else {
        // Otherwise, we're concerned with increasing
        // or decreasing timer values.
        if (buttonChange[0] != 0)
        {
            timer.updateResetTimer((timers[direction] += buttonChange[0]));
        }

        if (buttonChange[1] != 0)
        {
            timer.updateResetTimer((timers[direction] -= buttonChange[1]));
        }
    }

    TimeSpan interval = timer.time();
    int32_t totalseconds = interval.totalseconds();
    if (totalseconds > 0) {
        Serial.print("Interval:");
        Serial.println(totalseconds);
        int mins = interval.minutes();
        int secs = interval.seconds();

        // We need a consistent way to set digits
        int ones = mins % 10;
        mins = mins/10;
        int tens = mins % 10;
        lc.setDigit(0, 0, (byte)tens, false);
        lc.setDigit(0, 1, (byte)ones, true);

        ones = secs % 10;
        secs = secs/10;
        tens = secs % 10;
        lc.setDigit(0, 2, (byte)tens, false);
        lc.setDigit(0, 3, (byte)ones, false);
    }

    delay(250);
}
