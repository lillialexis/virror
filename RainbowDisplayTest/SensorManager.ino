
#include "Defines.h"

int rawScanArray[SCAN_WIDTH * SCAN_HEIGHT];

void scanSetup() {
    memset(rawScanArray, 0, SCAN_WIDTH * SCAN_HEIGHT * sizeof(int));
}

#ifdef DEV_SCAN
enum devDetectorTriggerStates {
    NOT_TRIGGERED_1,
    LEFT_TRIGGERED,
    NOT_TRIGGERED_2,
    RIGHT_TRIGGERED,
    NOT_TRIGGERED_3,
    BOTH_TRIGGERED,
    NUM_STATES
};

#define TRIGGERED_STATE_COUNTER_TIMEOUT     20
#define NOT_TRIGGERED_STATE_COUNTER_TIMEOUT 100

void readScan() {
    static int loopCounter = 0;
    static int modeCounter = 0;

    static int triggerCounter = 0;
    static int triggerState = NOT_TRIGGERED_1;

    if (triggerState % 2 == 0 &&
        triggerCounter == NOT_TRIGGERED_STATE_COUNTER_TIMEOUT) {

        triggerState = triggerState++ % NUM_STATES;
        triggerCounter = 0;

    } else if (triggerState % 2 == 1 &&
        triggerCounter == TRIGGERED_STATE_COUNTER_TIMEOUT) {

        triggerState = triggerState++ % NUM_STATES;
        triggerCounter = 0;
    }

    addData();

    if (triggerState == LEFT_TRIGGERED || triggerState == BOTH_TRIGGERED) {
        addLeftTrigger();
    }

    if (triggerState == RIGHT_TRIGGERED || triggerState == BOTH_TRIGGERED) {
        addRightTrigger();
    }
}

void addData() {

}

void addLeftTrigger() {
//    for (int i = 0; i < MODE_CHANGE_DETECTION_WIDTH; i++) {
//        for (int j = 0; j < MODE_CHANGE_DETECTION_HEIGHT; j++) { // TODO: If the scan data comes in from the bottom, flip these conditions
//            rawScanArray[rc2iScan(j, i)] = ;
//        }
//    }
}

void addRightTrigger() {

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
#define POS_COUNTER_TIMEOUT   10
#define NEG_COUNTER_TIMEOUT   5

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
