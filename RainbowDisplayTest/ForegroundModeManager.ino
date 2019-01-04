
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

void applyForeground(int scaledScanArray[], int *ledArray, int width, int height) {
    switch (foregroundMode) {
        case DarkMode:
            darken(scaledScanArray, ledArray, width, height);
            break;
        default:
            blue(scaledScanArray, ledArray, width, height);
            break;
    }
}

void blue(int scaledScanArray[], int ledArray[], int width, int height) {

}

void darken(int scaledScanArray[], int ledArray[], int width, int height) {

}

