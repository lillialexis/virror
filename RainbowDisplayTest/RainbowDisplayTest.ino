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

HSV ledArray[LED_WIDTH * LED_HEIGHT];
int scanArray[LED_WIDTH * LED_HEIGHT];

void setup() {
    pinMode(1, OUTPUT);
    digitalWrite(1, HIGH);

    Serial.begin(9600);

    memset(ledArray, 0, LED_WIDTH * LED_HEIGHT * sizeof(HSL));
    memset(scanArray, 0, LED_WIDTH * LED_HEIGHT * sizeof(int));

    scanSetup();
    backgroundSetup();
    foregroundSetup();
    feedbackSetup();

    digitalWrite(1, LOW);
    leds.begin();
}

void loop() {
    getFrame(scanArray, ledArray, LED_WIDTH, LED_HEIGHT);

    /* Set the pixels and loop.*/
    digitalWrite(1, HIGH);

    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            leds.setPixel(xy2i(x, y), makeColor(ledArray[rc2iLeds(y, x)]));
        }
    }

    leds.show();
    digitalWrite(1, LOW);
}



