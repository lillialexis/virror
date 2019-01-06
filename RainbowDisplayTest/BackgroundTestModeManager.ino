
#include "Defines.h"

#define CIRCLE_MODE_SHIFT_TIMEOUT 100

ModeVariants backgroundTestModeVariants = {0, 0, 0, 0, 0};

unsigned int backgroundTestMode = 0;
enum backgroundTestModes {
//    RAINBOW_BKG_TEST_MODE,
//    MOVING_CIRCLE_BKG_TEST_MODE,
//    RED_BKG_TEST_MODE,
//    RED_GRADIENT_BKG_TEST_MODE,
//    RED_SATURATION_GRADIENT_BKG_TEST_MODE,
//    RED_BRIGHTNESS_GRADIENT_BKG_TEST_MODE,
    COLOR_MIX_BKG_TEST_MODE,
    BACKGROUND_TEST_MODES_COUNT
};

void backgroundTestSetup() {

}

void setBackgroundTestModeVariants(ModeVariants modeVariants) {
    backgroundTestModeVariants = modeVariants;
}

void newTestBackgroundMode() {
    backgroundTestMode++;

    if (backgroundTestMode == BACKGROUND_TEST_MODES_COUNT) {
        backgroundTestMode = 0;
    }
}

void applyTestBackground(HSV ledArray[], unsigned int width, unsigned int height,
        unsigned int backgroundModeFrame) {

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

//        case RED_SATURATION_GRADIENT_BKG_TEST_MODE: {
//            redBkgSaturationGradientTestMode(ledArray, width, height);
//            break;
//        }

//        case RED_BRIGHTNESS_GRADIENT_BKG_TEST_MODE: {
//            redBkgBrightnessGradientTestMode(ledArray, width, height);
//            break;
//        }

        case COLOR_MIX_BKG_TEST_MODE: {
            colorMixBkgTestMode(ledArray, width, height, backgroundModeFrame);
            break;
        }

        default: {
            redBkgTestMode(ledArray, width, height);
            break;
        }
    }
}

void redBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            ledArray[rc2iLeds(y, x)] = {50, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
        }
    }
}

void redBkgGradientTestMode(HSV ledArray[], unsigned int width, unsigned int height) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            byte saturation = ((float) y / (float) height) * DEFAULT_SATURATION;
            byte value = ((float) x / (float) width) * DEFAULT_BRIGHTNESS;

            ledArray[rc2iLeds(y, x)] = {0, saturation, value};
        }
    }
}

void redBkgSaturationGradientTestMode(HSV ledArray[], unsigned int width, unsigned int height) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            byte saturation = ((float) x / (float) width) * DEFAULT_SATURATION;

            ledArray[rc2iLeds(y, x)] = {0, saturation, DEFAULT_BRIGHTNESS};
        }
    }
}

void redBkgBrightnessGradientTestMode(HSV ledArray[], unsigned int width, unsigned int height) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            byte value = ((float) x / (float) width) * DEFAULT_BRIGHTNESS;

            ledArray[rc2iLeds(y, x)] = {0, DEFAULT_SATURATION, value};
        }
    }
}

void colorMixBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height, unsigned int frame) {

}


void movingCircleBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height, unsigned int updateTimeout) {
    static unsigned int moveCounter = 0;
    static unsigned int color = 0;
    static Circle circle = {0.0, 0.0, width, height, 0.25, 1.0, LEFT, DOWN, 4.0};

    moveCounter++;
    if (moveCounter < updateTimeout) {
        return;
    }

    moveCircle(&circle);

    for (unsigned int x = 0; x < height; x++) {
        for (unsigned int y = 0; y < width; y++) {
            float value = getCircleVal(x, y, &circle);

            value = (1.0 - ((1.0 - value) * (1.0 - value)));

            if (value) {
                ledArray[rc2iLeds(x, y)] = {color % 255, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS - (value * MAX_BRIGHTNESS)};
            } else {
                ledArray[rc2iLeds(x, y)] = {0, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
            }
        }
    }

    color++;
    moveCounter = 0;
}

void rainbowBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            ledArray[rc2iLeds(y, x)] = {rc2iLeds(y, x) % 255, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS};
        }
    }
}
