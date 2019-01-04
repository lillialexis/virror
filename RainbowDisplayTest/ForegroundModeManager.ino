
#include "Defines.h"

void foregroundSetup() {

}

int foregroundMode = 0;
enum foregroundModes {
    DarkMode,
    ForegroundModesCount
};

void newForegroundMode() {
    foregroundMode++;

    if (foregroundMode == ForegroundModesCount) {
        foregroundMode = 0;
    }
}

void applyForeground(int scanArray[], int ledArray[], int width, int height) {
    switch (foregroundMode) {
        case DarkMode:
            darken(scanArray, ledArray, width, height);
            break;
        default:
            blue(scanArray, ledArray, width, height);
            break;
    }
}

void blue(int scanArray[], int ledArray[], int width, int height) {

}

void darken(int scaledScanArray[], int ledArray[], int width, int height) {

}

