
#include "Defines.h"

int rawScanArray[SCAN_WIDTH * SCAN_HEIGHT];

void scanSetup() {
    memset(rawScanArray, 0, SCAN_WIDTH * SCAN_HEIGHT * sizeof(int));
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

/* Lets put a couple of "detection windows" in the top-left and top-right areas of our scans.
 * If we detect something (e.g., a hand) in our detection window, trigger a mode change. The
 * scan data in our detection windows are added up and if the cumulative value passes the
 * MODE_CHANGE_DETECTION_THRESHOLD, a mode change is triggered. We can tweak these constants
 * to values that make sense and are the right amount of "sensitive" once we have the real scan
 * hardware set up. */
#define MODE_CHANGE_DETECTION_THRESHOLD 40
#define MODE_CHANGE_DETECTION_WIDTH     15
#define MODE_CHANGE_DETECTION_HEIGHT    3
#define POS_COUNT_THRESHOLD   10
#define NEG_COUNT_THRESHOLD   5

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
