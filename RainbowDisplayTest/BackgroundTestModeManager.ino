
#include "Defines.h"

#define CIRCLE_MODE_SHIFT_TIMEOUT 100

int backgroundTestMode = 0;
enum backgroundTestModes {
//    RAINBOW_BKG_TEST_MODE,
//    MOVING_CIRCLE_BKG_TEST_MODE,
//    RED_BKG_TEST_MODE,
//    RED_GRADIENT_BKG_TEST_MODE,
    RED_SATURATION_GRADIENT_BKG_TEST_MODE,
    RED_BRIGHTNESS_GRADIENT_BKG_TEST_MODE,
    BACKGROUND_TEST_MODES_COUNT
};

void newTestBackgroundMode() {
    backgroundTestMode++;

    if (backgroundTestMode == BACKGROUND_TEST_MODES_COUNT) {
        backgroundTestMode = 0;
    }
}

// TODO: Implement mode variants
void applyTestBackground(HSV ledArray[], int width, int height) {
    switch (backgroundTestMode) {

//        case RAINBOW_BKG_TEST_MODE: {
//            rainbowBkgTestMode(ledArray, width, height);
//            break;
//        }

//        case MOVING_CIRCLE_TEST_MODE: {
//            movingCircleTestMode(ledArray, width, height, CIRCLE_MODE_SHIFT_TIMEOUT);
//            break;
//        }

//        case RED_BKG_TEST_MODE: {
//            redBkgTestMode(ledArray, width, height);
//            break;
//        }

//        case RED_GRADIENT_BKG_TEST_MODE: {
//            redBkgGradientTestMode(ledArray, width, height);
//            break;
//        }

        case RED_SATURATION_GRADIENT_BKG_TEST_MODE: {
            redBkgSaturationGradientTestMode(ledArray, width, height);
            break;
        }

        case RED_BRIGHTNESS_GRADIENT_BKG_TEST_MODE: {
            redBkgBrightnessGradientTestMode(ledArray, width, height);
            break;
        }

        default: {
            redBkgTestMode(ledArray, width, height);
            break;
        }
    }
}

void redBkgTestMode(HSV ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            ledArray[rc2iLeds(y, x)] = {0, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
        }
    }
}

void redBkgGradientTestMode(HSV ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            int saturation = ((float) y / (float) LED_HEIGHT) * DEFAULT_SATURATION;
            int value = ((float) x / (float) LED_WIDTH) * DEFAULT_BRIGHTNESS;

            ledArray[rc2iLeds(y, x)] = {0, saturation, value};
        }
    }
}

void redBkgSaturationGradientTestMode(HSV ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            int saturation = ((float) x / (float) LED_WIDTH) * DEFAULT_SATURATION;
            //int value = ((float) x / (float) LED_WIDTH) * DEFAULT_BRIGHTNESS;

            ledArray[rc2iLeds(y, x)] = {0, saturation, DEFAULT_BRIGHTNESS};
        }
    }
}

void redBkgBrightnessGradientTestMode(HSV ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            //int saturation = ((float) y / (float) LED_HEIGHT) * DEFAULT_SATURATION;
            int value = ((float) x / (float) LED_WIDTH) * DEFAULT_BRIGHTNESS;

            ledArray[rc2iLeds(y, x)] = {0, DEFAULT_SATURATION, value};
        }
    }
}

void movingCircleBkgTestMode(HSV ledArray[], int width, int height, int updateTimeout) {
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
                ledArray[rc2iLeds(x, y)] = {color % 255, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS - value};
            } else {
                ledArray[rc2iLeds(x, y)] = {0, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
            }
        }
    }

    color++;
    moveCounter = 0;
}

void rainbowBkgTestMode(HSV ledArray[], int width, int height) {
    for (int x = 0; x < LED_WIDTH; x++) {
        for (int y = 0; y < LED_HEIGHT; y++) {
            ledArray[rc2iLeds(y, x)] = {rc2iLeds(y, x) % 255, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
        }
    }
}
