
#include "Defines.h"

void foregroundSetup() {

}

int foregroundMode = 0;
enum foregroundModes {
    DARK_MODE,
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
        default:
            blue(scanArray, ledArray, width, height);
            break;
    }
}

void blue(int scanArray[], HSV ledArray[], int width, int height) {

}

void darken(int scaledScanArray[], HSV ledArray[], int width, int height) {

}

