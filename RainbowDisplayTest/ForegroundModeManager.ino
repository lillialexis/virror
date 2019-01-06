
#include "Defines.h"

ModeVariants foregroundModeVariants = {0, 0, 0, 0, 0};

int foregroundMode = 0;
enum foregroundModes {
    DARK_MODE,
    BLUE_MODE,
    FOREGROUND_MODES_COUNT
};

void foregroundSetup() {

}

void setForegroundModeVariants(ModeVariants modeVariants) {
    foregroundModeVariants = modeVariants;
}

void newForegroundMode() {
    foregroundMode++;

    if (foregroundMode == FOREGROUND_MODES_COUNT) {
        foregroundMode = 0;
    }
}

void applyForeground(int scanArray[], HSV ledArray[], unsigned int width, unsigned int height,
        float foregroundAlpha, unsigned int foregroundModeFrame) {

    if (foregroundAlpha != 1.0) {
        applyForegroundAlpha(scanArray, width, height, foregroundAlpha);
    }

    switch (foregroundMode) {
        case DARK_MODE:
            darken(scanArray, ledArray, width, height);
            break;
        case BLUE_MODE:
            blue(scanArray, ledArray, width, height);
            break;
        default:
            blue(scanArray, ledArray, width, height);
            break;
    }
}

void applyForegroundAlpha(int scanArray[], unsigned int width, unsigned int height,
        float foregroundAlpha) {

    for (int i = 0; i < width * height; i++) {
        scanArray[i] = round(((float)scanArray[i]) * foregroundAlpha);
    }
}

void rainbow(int scanArray[], HSV ledArray[], int width, int height) {
    for (int i = 0; i < width * height; i++) {
        if (scanArray[i]) {
            HSV hsv = ledArray[i];

            hsv.h = 255 - (((float) scanArray[i] / 100.0) * 255.0);

            ledArray[i] = hsv;
        }
    }
}

void blue(int scanArray[], HSV ledArray[], int width, int height) {
    for (int i = 0; i < width * height; i++) {
        if (scanArray[i]) {
            HSV hsv = ledArray[i];

            hsv.h = 60;
            hsv.v = 255 - (((float) scanArray[i] / 100.0) * 255.0);

            ledArray[i] = hsv;
        }
    }
}

void darken(int scanArray[], HSV ledArray[], int width, int height) {
    for (int i = 0; i < width * height; i++) {
        if (scanArray[i]) {
            HSV hsv = ledArray[i];

            hsv.v = 255 - (((float) scanArray[i] / 100.0) * 255.0);

            ledArray[i] = hsv;
        }
    }
}

