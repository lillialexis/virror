
#include "Defines.h"

#define RAINBOW_MODE_PHASE_SHIFT   10
#define RAINBOW_MODE_SHIFT_TIMEOUT 40
#define CIRCLE_MODE_SHIFT_TIMEOUT  100

int rainbowColors[180];
int backgroundMode = 0;

enum backgroundModes {
        //RAINBOW_MODE,
        CIRCLE_MODE,
        //RED_MODE,
        //RED_GRADIENT_MODE,
        BACKGROUND_MODES_COUNT
};

void backgroundSetup() {
    for (int i =  0; i< 180; i++) {
        int hue = i * 2;
        // pre-compute the 180 rainbow colors
        rainbowColors[i] = hue;//makeColor(hue, saturation, lightness);
    }
}

void newBackgroundMode() {
    backgroundMode++;

    if (backgroundMode == BACKGROUND_MODES_COUNT) {
        backgroundMode = 0;
    }
}

// TODO: Implement mode variants
void applyBackground(HSL ledArray[], int width, int height) {//, int modeVariant1, int modeVariant2, int modeVariant3) {
    switch (backgroundMode) {
//        case RAINBOW_MODE: {
//            rainbow(ledArray, width, height, RAINBOW_MODE_PHASE_SHIFT, RAINBOW_MODE_SHIFT_TIMEOUT);
//            break;
//        }

        case CIRCLE_MODE: {
            movingCircle(ledArray, width, height, CIRCLE_MODE_SHIFT_TIMEOUT);
            break;
        }

//        case RED_MODE: {
//            red(ledArray, width, height);
//            break;
//        }
//
//        case RED_GRADIENT_MODE: {
//            redGradient(ledArray, width, height);
//            break;
//        }

        default: {
            red(ledArray, width, height);
            break;
        }
    }
}

void red(HSL ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            ledArray[rc2iLeds(y, x)] = {0, DEFAULT_SATURATION, DEFAULT_LIGHTNESS};
        }
    }
}

void redGradient(HSL ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            int saturation = ((float) x / (float) LED_WIDTH) * 100;
            int lightness = ((float) y / (float) LED_HEIGHT) * 100;

            ledArray[rc2iLeds(y, x)] = {0, saturation, lightness};
        }
    }
}

void movingCircle(HSL ledArray[], int width, int height, int updateTimeout) {
    static int moveCounter = 0;
    static int color = 0;
    static Circle circle = {0.0, 0.0, LED_WIDTH, LED_HEIGHT, 0.25, 1.0, LEFT, DOWN, 4.0};

    moveCounter++;
    if (moveCounter < updateTimeout) {
        return;
    }

    moveCircle(&circle);

    for (int x = 0; x < LED_HEIGHT; x++) {
        for (int y = 0; y < LED_WIDTH; y++) {
            int value = getCircleVal(x, y, &circle);

            if (value) {
                ledArray[rc2iLeds(x, y)] = {color%360, value, 50 - value / 2};
            } else {
                ledArray[rc2iLeds(x, y)] = {0, DEFAULT_SATURATION, DEFAULT_LIGHTNESS};
            }
        }
    }

    color++;
    moveCounter = 0;
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
void rainbow(HSL ledArray[], int width, int height, int phaseShift, int updateTimeout) {
    static int color = 0;
    static int colorChangeCounter = updateTimeout; // Setting this to max to set the initial colors

    colorChangeCounter++;

    if (colorChangeCounter < updateTimeout) {
        return;
    }

    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            int index = (color + x + y*phaseShift/2) % 180;

            ledArray[rc2iLeds(y, x)] = {rainbowColors[index], DEFAULT_SATURATION, DEFAULT_LIGHTNESS};
        }
    }

    // TODO: Seems to have a weirdly shortened red band; fix
    color++;
    colorChangeCounter = 0;
}
