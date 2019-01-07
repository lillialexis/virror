
#ifdef USING_BKG_TEST_MODES

#include "Defines.h"

#define CIRCLE_MODE_SHIFT_TIMEOUT 100

ModeVariants backgroundTestModeVariants = {0, 0, 0, 0, 0};

unsigned int backgroundTestMode = 0;
enum backgroundTestModes {
//    RAINBOW_BKG_TEST_MODE,
//    MOVING_CIRCLE_BKG_TEST_MODE,
//    RED_BKG_TEST_MODE,
    GRADIENT_BKG_TEST_MODE,
    SATURATION_BKG_TEST_MODE,
    BRIGHTNESS_BKG_TEST_MODE,
//    COLOR_MIX_BKG_TEST_MODE,
//    COLOR_TRANSITION_BKG_TEST_MODE,
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

        case GRADIENT_BKG_TEST_MODE: {
            gradientBkgTestMode(ledArray, width, height, backgroundModeFrame);
            break;
        }

        case SATURATION_BKG_TEST_MODE: {
            saturationBkgTestMode(ledArray, width, height, backgroundModeFrame);
            break;
        }

        case BRIGHTNESS_BKG_TEST_MODE: {
            brightnessBkgTestMode(ledArray, width, height, backgroundModeFrame);
            break;
        }

//        case COLOR_MIX_BKG_TEST_MODE: {
//            colorMixBkgTestMode(ledArray, width, height, backgroundModeFrame);
//            break;
//        }

//        case COLOR_TRANSITION_BKG_TEST_MODE: {
//            colorTransitionBkgTestMode(ledArray, width, height, backgroundModeFrame);
//            break;
//        }

        default: {
            redBkgTestMode(ledArray, width, height);
            break;
        }
    }
}


void redBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            ledArray[rc2iLeds(y, x)] = { 0, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };
        }
    }
}

byte btmHueScroll(unsigned int frame) {
    return ((frame / 200) % 255);
}

void gradientBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height, unsigned int frame) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            byte saturation = (((float) y + 1) / (float) height) * DEFAULT_SATURATION;
            byte value = (((float) x + 1) / (float) width) * DEFAULT_BRIGHTNESS;

            ledArray[rc2iLeds(y, x)] = { btmHueScroll(frame), saturation, value };
        }
    }
}

void saturationBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height, unsigned int frame) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            byte saturation = ((float) x / (float) width) * DEFAULT_SATURATION;

            ledArray[rc2iLeds(y, x)] = { btmHueScroll(frame), saturation, DEFAULT_BRIGHTNESS };
        }
    }
}

void brightnessBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height, unsigned int frame) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            byte value = ((float) x / (float) width) * DEFAULT_BRIGHTNESS;

            ledArray[rc2iLeds(y, x)] = { btmHueScroll(frame), DEFAULT_SATURATION, value };
        }
    }
}

void colorMixBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height, unsigned int frame) {
    static unsigned int counter1 = 0;
    static unsigned int counter2 = 0;

    if ((frame + 1) % 750 == 0) {// TODO: If we add code to drop slow frames, this won't work if the 50th frame is dropped; fix
        counter1 = ppm(counter1, MAX_SIMPLE_COLORS);

        if (counter1 == MAX_SIMPLE_COLORS - 1) {
            counter2 = ppm(counter2, MAX_SIMPLE_COLORS);
        }
    }

    for (unsigned int x = 0; x < height; x++) {
        for (unsigned int y = 0; y < width; y++) {
            HSV h1 = SIMPLE_COLORS[counter2];
            HSV h2 = SIMPLE_COLORS[counter1];
            HSV h3 = { 0, 0, 0 };

            if (x < 2 || x >= (width - 2)) {
                h3 = h1;
            } else if (x < (width / 2)) {
                h3 = mixHsv(h1, h2, 1.0);
            } else {
                h3 = mixHsv(h1, h2, (1.0 - (float)(x - (width/2) + 1)/(float)((width/2) - 1)));
            }

            ledArray[rc2iLeds(y, x)] = h3;
        }
    }
}

void colorTransitionBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height, unsigned int frame) {
    static unsigned int counter1 = 0;
    static unsigned int counter2 = 1;

    const unsigned int  timeout = 750;
    static unsigned int current = 0;

    if ((frame + 1) % timeout == 0) {// TODO: If we add code to drop slow frames, this won't work if the 50th frame is dropped; fix
        counter1 = ppm(counter1, MAX_SIMPLE_COLORS);
        counter2 = ppm(counter1, MAX_SIMPLE_COLORS);
        current = 0;
    }

    for (unsigned int x = 0; x < height; x++) {
        for (unsigned int y = 0; y < width; y++) {
            HSV h1 = SIMPLE_COLORS[counter1];
            HSV h2 = SIMPLE_COLORS[counter2];

            ledArray[rc2iLeds(y, x)] = transitionHsv(h1, h2, (float)current/(float)timeout);
        }
    }

    current++;
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
                ledArray[rc2iLeds(x, y)] = { color % 255, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS - (value * MAX_BRIGHTNESS) };
            } else {
                ledArray[rc2iLeds(x, y)] = { 0, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };
            }
        }
    }

    color++;
    moveCounter = 0;
}

void rainbowBkgTestMode(HSV ledArray[], unsigned int width, unsigned int height) {
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            ledArray[rc2iLeds(y, x)] = { rc2iLeds(y, x) % 255, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };
        }
    }
}

#endif
