
#include "Defines.h"

#define RAINBOW_MODE_PHASE_SHIFT   10
#define RAINBOW_MODE_SHIFT_TIMEOUT 40

int rainbowColors[127];

int backgroundMode = 0;
enum backgroundModes {
        RAINBOW_BKG_MODE,
        RED_BKG_MODE,
        BACKGROUND_MODES_COUNT
};

void backgroundSetup() {
    for (int i =  0; i < 127; i++) {
        int hue = i * 2;
        // pre-compute the 127 rainbow colors
        rainbowColors[i] = hue;
    }
}

void newBackgroundMode() {

#ifdef USING_BKG_TEST_MODES
    newTestBackgroundMode();
#else

    backgroundMode++;

    if (backgroundMode == BACKGROUND_MODES_COUNT) {
        backgroundMode = 0;
    }
#endif
}

// TODO: Implement mode variants
void applyBackground(HSV ledArray[], int width, int height) {//, int modeVariant1, int modeVariant2, int modeVariant3) {

#ifdef USING_BKG_TEST_MODES
    applyTestBackground(ledArray, width, height);
#else

    switch (backgroundMode) {
        case RAINBOW_BKG_MODE: {
            rainbowBkgMode(ledArray, width, height, RAINBOW_MODE_PHASE_SHIFT, RAINBOW_MODE_SHIFT_TIMEOUT);
            break;
        }

        case RED_BKG_MODE: {
            redBkgMode(ledArray, width, height);
            break;
        }

        default: {
            redBkgMode(ledArray, width, height);
            break;
        }
    }
#endif
}

void redBkgMode(HSV ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            ledArray[rc2iLeds(y, x)] = {0, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
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
void rainbowBkgMode(HSV ledArray[], int width, int height, int phaseShift, int updateTimeout) {
    static int color = 0;
    static int colorChangeCounter = updateTimeout; // Setting this to max to set the initial colors

    colorChangeCounter++;

    if (colorChangeCounter < updateTimeout) {
        return;
    }

    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            int index = (color + x + y * phaseShift / 2) % 127;

            ledArray[rc2iLeds(y, x)] = {rainbowColors[index], DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
        }
    }

    // TODO: Seems to have a weirdly shortened red band; fix
    color++;
    colorChangeCounter = 0;
}
