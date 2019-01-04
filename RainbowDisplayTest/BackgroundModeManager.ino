
#include "Defines.h"

#define RAINBOW_MODE_PHASE_SHIFT 10

int rainbowColors[180];
int backgroundMode = 0;

enum backgroundModes {
        RainbowMode,
        BackgroundModesCount
};

void backgroundSetup() {
    for (int i =  0; i< 180; i++) {
        int hue = i * 2;
        int saturation = 100;
        int lightness = 50;
        // pre-compute the 180 rainbow colors
        rainbowColors[i] = hue;//makeColor(hue, saturation, lightness);
    }
}

void newBackgroundMode() {
    backgroundMode++;

    if (backgroundMode == BackgroundModesCount) {
        backgroundMode = 0;
    }
}

void applyBackground(int ledArray[], int width, int height) {
    switch (backgroundMode) {
        case RainbowMode:
            rainbow(ledArray, width, height, RAINBOW_MODE_PHASE_SHIFT);
            break;
        default:
            red(ledArray, width, height);
            break;
    }
}

void red(int ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            ledArray[rc2iLeds(y, x)] = 0;
        }
    }
}

// phaseShift is the shift between each row.  phaseShift=0
// causes all rows to show the same colors moving together.
// phaseShift=180 causes each row to be the opposite colors
// as the previous.
//
// cycleTime is the number of milliseconds to shift through // TODO: Reconcile this w new architecture
// the entire 360 degrees of the color wheel:
// Red -> Orange -> Yellow -> Green -> Blue -> Violet -> Red
//
void rainbow(int ledArray[], int width, int height, int phaseShift) {
    // TODO: Why this outer loop?

    for (int color = 0; color < 180; color++) {
        for (int x = 0; x < LED_WIDTH; x++) {
            for (int y = 0; y < LED_HEIGHT; y++) {
                int index = (color + x + y*phaseShift/2) % 180;

                ledArray[rc2iLeds(y, x)] = rainbowColors[index];
            }
        }
    }
}
