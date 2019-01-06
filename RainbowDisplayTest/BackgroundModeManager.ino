#include <math.h>
#include "Defines.h"

#define RAINBOW_MODE_PHASE_SHIFT   10
#define RAINBOW_MODE_SHIFT_TIMEOUT 20

ModeVariants backgroundModeVariants = {0, 0, 0, 0, 0}; // TODO: Am I stepping on memory with the way this is implemented?

//int rainbowColors[127];

int backgroundMode = 0;
enum backgroundModes {
        RAINBOW_BKG_MODE,
        RED_BKG_MODE,
        BACKGROUND_MODES_COUNT
};

void backgroundSetup() {

#ifdef USING_BKG_TEST_MODES
    backgroundTestSetup();
#else

//    for (int i =  0; i < 127; i++) {
//        /* Pre-compute the 127 rainbow colors */
//        rainbowColors[i] = i * 2;
//    }
#endif
}

void setBackgroundModeVariants(ModeVariants modeVariants) {

#ifdef USING_BKG_TEST_MODES
    setBackgroundTestModeVariants(modeVariants);
#else

    backgroundModeVariants = modeVariants;
#endif
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

void applyBackground(HSV ledArray[], unsigned int width, unsigned int height,
        float backgroundAlpha, unsigned int backgroundModeFrame) {

#ifdef USING_BKG_TEST_MODES
    applyTestBackground(ledArray, width, height, backgroundModeFrame);
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

    if (backgroundAlpha != 1.0) {
        applyBackgroundAlpha(ledArray, width, height, backgroundAlpha);
    }
}

void applyBackgroundAlpha(HSV ledArray[], unsigned int width, unsigned int height,
        float backgroundAlpha) {

    for (int i = 0; i < width * height; i++) {
        ledArray[i] =
                { ledArray[i].h, ledArray[i].s, round(((float)(ledArray[i].v)) * backgroundAlpha) };
    }
}

void redBkgMode(HSV ledArray[], int width, int height) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
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
    static unsigned int colorIndex = 0;
    static unsigned int colorChangeCounter = 0;

    if (colorChangeCounter >= updateTimeout) {
        colorIndex++;
        colorChangeCounter = 0;
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            unsigned int color = (colorIndex + x + y * phaseShift / 2) % 255;

            ledArray[rc2iLeds(y, x)] = {color, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
        }
    }

    colorChangeCounter++;

    // TODO: Seems to have a weirdly shortened red band; fix
}
