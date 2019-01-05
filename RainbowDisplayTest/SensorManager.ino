
#include "Defines.h"


int rawScanArray[SCAN_WIDTH * SCAN_HEIGHT];

void scanSetup() {
    memset(rawScanArray, 0, SCAN_WIDTH * SCAN_HEIGHT * sizeof(int));
}

#ifdef REAL_SCAN
void readScan() {
    // TODO: Read real scan data into scanArray
}

ModeVariants getModeVariants() {
    // TODO: Implement
    return {1, 2, 3, 4, 255};
}
#endif

bool checkForegroundModeChange() {
    static int positiveCounterFgd = 0;
    static int negativeCounterFgd = 0;

    int countTotal = 0;

    for (int i = 0; i < MODE_CHANGE_DETECTION_WIDTH; i++) {
        for (int j = 0; j < MODE_CHANGE_DETECTION_HEIGHT; j++) { // TODO: If the scan data comes in from the bottom, flip these conditions
            countTotal += rawScanArray[rc2iScan(j, i)];
        }
    }

    if (countTotal > MODE_CHANGE_DETECTION_THRESHOLD) {
        positiveCounterFgd++;
    } else {
        negativeCounterFgd++;
    }

    if (positiveCounterFgd > POS_COUNTER_TIMEOUT) {
        positiveCounterFgd = 0;
        negativeCounterFgd = 0;

        return true;
    } else if (negativeCounterFgd > NEG_COUNTER_TIMEOUT) {
        positiveCounterFgd = 0;
        negativeCounterFgd = 0;

        return false;
    } else {
        return false;
    }
}

bool checkBackgroundModeChange() {
    static int positiveCounterBgd = 0;
    static int negativeCounterBgd = 0;

    int countTotal = 0;

    for (int i = SCAN_WIDTH - MODE_CHANGE_DETECTION_WIDTH; i < SCAN_WIDTH; i++) {
        for (int j = 0; j < MODE_CHANGE_DETECTION_HEIGHT; j++) { // TODO: If the scan data comes in from the bottom, flip these conditions
            countTotal += rawScanArray[rc2iScan(j, i)];
        }
    }

    if (countTotal > MODE_CHANGE_DETECTION_THRESHOLD) {
        positiveCounterBgd++;
    } else {
        negativeCounterBgd++;
    }

    if (positiveCounterBgd > POS_COUNTER_TIMEOUT) {
        positiveCounterBgd = 0;
        negativeCounterBgd = 0;

        return true;
    } else if (negativeCounterBgd > NEG_COUNTER_TIMEOUT) {
        positiveCounterBgd = 0;
        negativeCounterBgd = 0;

        return false;
    } else {
        return false;
    }
}

void scaleScan(int scaledScanArray[], int width, int height) {
    // TODO: Scale for real

    for (int i = 0; i < width * height; i++) {
        scaledScanArray[i] = rawScanArray[i];
    }
}

int scan(int scaledScanArray[], int width, int height) {
    int modeChangeFlag = NO_MODE_CHANGE;

    readScan();

    if (checkForegroundModeChange()) {
        modeChangeFlag |= FOREGROUND_MODE_CHANGE_FLAG;
    }

    if (checkBackgroundModeChange()) {
        modeChangeFlag |= BACKGROUND_MODE_CHANGE_FLAG;
    }

    scaleScan(scaledScanArray, width, height);

    return modeChangeFlag;
}
