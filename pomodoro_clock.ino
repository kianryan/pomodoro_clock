#include "LedControl.h"
#include "RTClib.h"
#include "PomoTimer.h"

RTC_DS1307 rtc;
LedControl lc = LedControl(12, 11, 10, 1);
PomoTimer upTimer(300);

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

   upTimer.setClock(&rtc);
   upTimer.start();
}

void loop() {

    TimeSpan interval = upTimer.time();
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
    } else {
        // Restart the clock
        upTimer.start();
    }

    delay(250);
}
