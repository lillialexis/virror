
#include "Defines.h"

int scanArray[SCAN_WIDTH * SCAN_HEIGHT];

void scanSetup() {
    memset(scanArray, 0, sizeof(int));
}

#ifdef DEV_SCAN
void readScan() {
    static int loopCounter = 0;
    static int modeCounter = 0;

    // TODO
}
#endif

#ifdef REAL_SCAN
void readScan() {
    // TODO: Read real scan data into scanArray
}
#endif

#define MODE_CHANGE_THRESHOLD 20
#define POS_COUNT_THRESHOLD   10
#define NEG_COUNT_THRESHOLD   5

bool checkForegroundModeChange() {
    static int positiveCounterFgd = 0;
    static int negativeCounterFgd = 0;

    int countTotal = 0;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 2; j++) {
            countTotal += scanArray[rc2iScan(j, i)];
        }
    }

    if (countTotal > MODE_CHANGE_THRESHOLD) {
        positiveCounterFgd++;
    } else {
        negativeCounterFgd++;
    }

    if (positiveCounterFgd > POS_COUNT_THRESHOLD) {
        positiveCounterFgd = 0;
        negativeCounterFgd = 0;

        return true;
    } else if (negativeCounterFgd > NEG_COUNT_THRESHOLD) {
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

    for (int i = 70; i < 80; i++) {
        for (int j = 0; j < 2; j++) {
            countTotal += scanArray[rc2iScan(j, i)];
        }
    }

    if (countTotal > MODE_CHANGE_THRESHOLD) {
        positiveCounterBgd++;
    } else {
        negativeCounterBgd++;
    }

    if (positiveCounterBgd > POS_COUNT_THRESHOLD) {
        positiveCounterBgd = 0;
        negativeCounterBgd = 0;

        return true;
    } else if (negativeCounterBgd > NEG_COUNT_THRESHOLD) {
        positiveCounterBgd = 0;
        negativeCounterBgd = 0;

        return false;
    } else {
        return false;
    }
}

void scaleScan(int scaledScanArray[], int width, int height) {
    // TODO
}

int scan(int scaledScanArray[], int width, int height) {
    int modeChangeFlag = 0;

    readScan();

    if (checkForegroundModeChange()) {
        modeChangeFlag |= FOREGROUND_CHANGE;
    }

    if (checkBackgroundModeChange()) {
        modeChangeFlag |= BACKGROUND_CHANGE;
    }

    scaleScan(scaledScanArray, width, height);

    return modeChangeFlag;
}
