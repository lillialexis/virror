
#include "Defines.h"

#define RAINBOW_MODE_PHASE_SHIFT 10
#define RAINBOW_MODE_CYCLE_TIME  40

int rainbowColors[180];
int backgroundMode = 0;

enum backgroundModes {
        RainbowMode,
        RedMode,
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

// TODO: Implement mode variants
void applyBackground(int ledArray[], int width, int height) {//, int modeVariant1, int modeVariant2, int modeVariant3) {
    switch (backgroundMode) {
        case RainbowMode: {
            rainbow(ledArray, width, height, RAINBOW_MODE_PHASE_SHIFT, RAINBOW_MODE_CYCLE_TIME);
            break;
        }

        case RedMode: {
            red(ledArray, width, height);
            break;
        }

        default: {
            red(ledArray, width, height);
            break;
        }
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
void rainbow(int ledArray[], int width, int height, int phaseShift, int cycleTime) {
    //int waitThreshold = cycleTime / (width * height);

    static int color = 0;
    static int colorChangeCounter = cycleTime;

    colorChangeCounter++;

    if (colorChangeCounter < cycleTime) {// waitThreshold) {
        return;
    }

    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            int index = (color + x + y*phaseShift/2) % 180;

            ledArray[rc2iLeds(y, x)] = rainbowColors[index];
        }
    }

    color = (color + 1) % 180;
    colorChangeCounter = 0;
}
