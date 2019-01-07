
#include "Defines.h"

ModeVariants foregroundModeVariants = {0, 0, 0, 0, 0};

int foregroundMode = 0;
enum foregroundModes {
    //RAINBOW_FGD_MODE,
//    DARK_FGD_MODE,
//    LIGHT_FGD_MODE,
            BLUE_FGD_MODE,
    GREEN_FGD_MODE,
    FOREGROUND_MODES_COUNT
};

void foregroundSetup() {

}

void setForegroundModeVariants(ModeVariants modeVariants) {
    foregroundModeVariants.modeVariant1 = modeVariants.modeVariant1;
    foregroundModeVariants.modeVariant2 = modeVariants.modeVariant2;
    foregroundModeVariants.modeVariant3 = modeVariants.modeVariant3;
    foregroundModeVariants.modeVariant4 = modeVariants.modeVariant4;
    foregroundModeVariants.modeVariant5 = modeVariants.modeVariant5;
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
//        case DARK_FGD_MODE:
//            darkenFgdMode(scanArray, ledArray, width, height);
//            break;
//        case LIGHT_FGD_MODE:
//            lightedFgdMode(scanArray, ledArray, width, height);
//            break;
        case BLUE_FGD_MODE:
            blueFgdMode(scanArray, ledArray, width, height);
            break;
        case GREEN_FGD_MODE:
            greenFgdMode(scanArray, ledArray, width, height);
            break;
        default:
            blueFgdMode(scanArray, ledArray, width, height);
            break;
    }
}

void applyForegroundAlpha(int scanArray[], unsigned int width, unsigned int height,
        float foregroundAlpha) {

    for (int i = 0; i < width * height; i++) {
        scanArray[i] = round(((float)scanArray[i]) * foregroundAlpha);
    }
}

void rainbowFgdMode(int scanArray[], HSV ledArray[], int width, int height) {
    for (int i = 0; i < width * height; i++) {
        if (scanArray[i]) {
            HSV hsv1 = ledArray[i];
            HSV hsv2 = { 255 - (((float) scanArray[i] / MAX_SCAN_VALUE) * 255.0), DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };

            ledArray[i] = mixHsv(hsv1, hsv2, ((float) scanArray[i] / MAX_SCAN_VALUE));
        }
    }
}

void darkenFgdMode(int scanArray[], HSV ledArray[], int width, int height) {
    for (int i = 0; i < width * height; i++) {
        if (scanArray[i]) {
            HSV hsv1 = ledArray[i];
            HSV hsv2 = { 0, DEFAULT_SATURATION, 0 };

            ledArray[i] = mixHsv(hsv1, hsv2, ((float) scanArray[i] / MAX_SCAN_VALUE));
        }
    }
}

void lightedFgdMode(int scanArray[], HSV ledArray[], int width, int height) {
    for (int i = 0; i < width * height; i++) {
        if (scanArray[i]) {
            HSV hsv1 = ledArray[i];
            HSV hsv2 = { 0, 0, DEFAULT_BRIGHTNESS };

            ledArray[i] = mixHsv(hsv1, hsv2, ((float) scanArray[i] / MAX_SCAN_VALUE));
        }
    }
}

void blueFgdMode(int scanArray[], HSV ledArray[], int width, int height) {
    for (int i = 0; i < width * height; i++) {
        if (scanArray[i]) {
            HSV hsv1 = ledArray[i];
            HSV hsv2 = { 170, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };

            ledArray[i] = mixHsv(hsv1, hsv2, ((float) scanArray[i] / MAX_SCAN_VALUE));
        }
    }
}

void greenFgdMode(int scanArray[], HSV ledArray[], int width, int height) {
    for (int i = 0; i < width * height; i++) {
        if (scanArray[i]) {
            HSV hsv1 = ledArray[i];
            HSV hsv2 = { 85, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };

            ledArray[i] = mixHsv(hsv1, hsv2, ((float) scanArray[i] / MAX_SCAN_VALUE));
        }
    }
}

