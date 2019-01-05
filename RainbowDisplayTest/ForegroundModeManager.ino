
#include "Defines.h"

void foregroundSetup() {

}

int foregroundMode = 0;
enum foregroundModes {
    DARK_MODE,
    BLUE_MODE,
    FOREGROUND_MODES_COUNT
};

void newForegroundMode() {
    foregroundMode++;

    if (foregroundMode == FOREGROUND_MODES_COUNT) {
        foregroundMode = 0;
    }
}

void applyForeground(int scanArray[], HSV ledArray[], int width, int height) {
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

