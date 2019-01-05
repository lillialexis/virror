/*  OctoWS2811 Rainbow.ino - Rainbow Shifting Test
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.


  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.
    pin 1 - Output indicating CPU usage, monitor with an oscilloscope,
            logic analyzer or even an LED (brighter = CPU busier)
*/

#include <OctoWS2811.h>
#include "Defines.h"

const int ledsPerPin = LED_WIDTH * LED_HEIGHT / 8;
const int ledsPerStrip = ledsPerPin;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

HSL ledArray[LED_WIDTH * LED_HEIGHT];
int scanArray[LED_WIDTH * LED_HEIGHT];

//int rainbowColors[180];
// The display size and color to use

//const unsigned int scan_width = 80;
//const unsigned int scan_height = 14;
//
//int alphaArray[scan_width * scan_height];
//
//int rc2i(int row, int col) {
//  return (row * scan_width) + col;
//}

void setup() {
    pinMode(1, OUTPUT);
    digitalWrite(1, HIGH);
//  for (int i=0; i<180; i++) {
//    int hue = i * 2;
//    int saturation = 100;
//    int lightness = 50;
//    // pre-compute the 180 rainbow colors
//    rainbowColors[i] = hue;//makeColor(hue, saturation, lightness);
//  }

//  for (int i = 0; i < scan_height; i++) {
//    for (int j = 0; j < scan_width; j++) {
//      if (i > 3 && i < 10 && j > 20 && j < 70) {
//        alphaArray[rc2i(i, j)] = 50;
//      } else {
//        alphaArray[rc2i(i, j)] = 50;//0;
//      }
//    }
//  }

    memset(ledArray, 0, LED_WIDTH * LED_HEIGHT * sizeof(HSL));
    memset(scanArray, 0, LED_WIDTH * LED_HEIGHT * sizeof(int));

    scanSetup();
    backgroundSetup();
    foregroundSetup();

    digitalWrite(1, LOW);
    leds.begin();
}

void loop() {
    /* Initialize the background- and foreground-mode change counters. */
    static unsigned int backgroundModeChangeCounter = (MODE_CHANGE_COUNTER_TIMEOUT / 2);
    static unsigned int foregroundModeChangeCounter = 0;

    /* Get the scan. Did our scan detect a mode change? */
    int modeMask = scan(scanArray, LED_WIDTH, LED_HEIGHT);

    /* If the scan detected a background-mode change or if our background-mode change counter hit
     * the threshold, change the background mode. */
    if (modeMask & BACKGROUND_CHANGE ||
        backgroundModeChangeCounter == MODE_CHANGE_COUNTER_TIMEOUT) {
            newBackgroundMode();

            backgroundModeChangeCounter = 0;
    }

    /* If the scan detected a foreground-mode change or if our foreground-mode change counter hit
     * the threshold, change the foreground mode. */
    if (modeMask & FOREGROUND_CHANGE ||
        foregroundModeChangeCounter == MODE_CHANGE_COUNTER_TIMEOUT) {
            newForegroundMode();

            foregroundModeChangeCounter = 0;
    }

    /* Increase the background- and foreground-mode change counters. */
    foregroundModeChangeCounter++;
    backgroundModeChangeCounter++;

    /* Apply the loop's new background, then mix with the loop's new foreground. */
    applyBackground(ledArray, LED_WIDTH, LED_HEIGHT);
    applyForeground(scanArray, ledArray, LED_WIDTH, LED_HEIGHT);

    /* Set the pixels and loop.*/
    digitalWrite(1, HIGH);

    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            leds.setPixel(xy(x, y), makeColor(ledArray[rc2iLeds(y, x)]));//, 100, 50));
        }
    }

    leds.show();
    digitalWrite(1, LOW);
}

//int applyAlpha(int oldColor, int aVal) {
//    return makeColor(oldColor, 100, aVal);
//
////  if (aVal) {
////    return 0;
////  }
////
////  return oldColor;
//}
//
//int aValFromArray(int x, int y) {
//  int newY = ((float) y) / 3.43;
//  int newX = ((float) x) * 1.25 * 2.0;
//
//  return alphaArray[rc2i(newY, newX)];
//}


//// phaseShift is the shift between each row.  phaseShift=0
//// causes all rows to show the same colors moving together.
//// phaseShift=180 causes each row to be the opposite colors
//// as the previous.
////
//// cycleTime is the number of milliseconds to shift through
//// the entire 360 degrees of the color wheel:
//// Red -> Orange -> Yellow -> Green -> Blue -> Violet -> Red
////
//void rainbow(int phaseShift, int cycleTime)
//{
//  int color, x, y, offset, wait;
//
//  wait = cycleTime * 1000 / ledsPerStrip;
//  for (color=0; color < 180; color++) {
//    digitalWrite(1, HIGH);
//    for (x=0; x < LED_WIDTH; x++) {
//      for (y=0; y < LED_HEIGHT; y++) {
//        int index = (color + x + y*phaseShift/2) % 180;
////        leds.setPixel(x + y*ledsPerStrip, rainbowColors[index]);
//
//        int origColor = rainbowColors[index];
//        int aVal = aValFromArray(x, y);
//        int newColor = applyAlpha(origColor, aVal);
//
//        leds.setPixel(xy(x,y), newColor);
//      }
//    }
//    leds.show();
//    digitalWrite(1, LOW);
//    delayMicroseconds(wait);
//  }
//}

// TODO: Probably move to util.ino
unsigned int xy(unsigned int x, unsigned int y) {
    bool LorR;
    y = LED_HEIGHT - y - 1; // invert display

    if (y % 16 < 8) // 2 * 8 strands run back and fourth
        { LorR = true; } // line goes LtoR
    else { LorR = false; } // line goes RtoL

    unsigned int channel_index = (y % 8) * ledsPerPin; // array index for start of the LED strand of the LED in question
    unsigned int strand_index = (y / 8) * LED_WIDTH;

    if (LorR) { return channel_index + strand_index + x; }
    else { return channel_index + strand_index + LED_WIDTH - x - 1; }
}

