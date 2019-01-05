
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

#define TRIGGERED_STATE_COUNTER_TIMEOUT     2000
#define NOT_TRIGGERED_STATE_COUNTER_TIMEOUT 10000

void readScan() {
    static int triggerCounter = 0;
    static int triggerState = NOT_TRIGGERED_1;

    if (triggerState % 2 == 0 &&
        triggerCounter == NOT_TRIGGERED_STATE_COUNTER_TIMEOUT) {

        triggerState = (triggerState + 1) % NUM_STATES;
        triggerCounter = 0;

    } else if (triggerState % 2 == 1 &&
        triggerCounter == TRIGGERED_STATE_COUNTER_TIMEOUT) {

        triggerState = (triggerState + 1) % NUM_STATES;
        triggerCounter = 0;
    }

    addData();

//    if (triggerState == LEFT_TRIGGERED || triggerState == BOTH_TRIGGERED) {
//        addLeftTrigger();
//    }
//
//    if (triggerState == RIGHT_TRIGGERED || triggerState == BOTH_TRIGGERED) {
//        addRightTrigger();
//    }

    triggerCounter++;
}

#define MOVE_CIRCLE_TIMEOUT 20
void addData() {
    static int moveCounter = 0;
    static Circle circle = {0.0, 0.0, SCAN_WIDTH, SCAN_HEIGHT, 0.25, 1.0, LEFT, DOWN, 4.0};

    moveCounter++;
    if (moveCounter < MOVE_CIRCLE_TIMEOUT) {
        return;
    }

    moveCircle(&circle);

    for (int x = 0; x < SCAN_HEIGHT; x++) {
        for (int y = 0; y < SCAN_WIDTH; y++) {
            int value = getCircleVal(x, y, &circle);

            if (value) {
                rawScanArray[rc2iScan(x, y)] = value;
            } else {
                rawScanArray[rc2iScan(x, y)] = 0;
            }
        }
    }

    moveCounter = 0;
}

void addLeftTrigger() {
    static Circle circle =
            {MODE_CHANGE_DETECTION_WIDTH / 2,
             MODE_CHANGE_DETECTION_HEIGHT / 2,
             SCAN_WIDTH, SCAN_HEIGHT, 0, 0, LEFT, DOWN,
             MODE_CHANGE_DETECTION_HEIGHT / 2};

    for (int i = 0; i < MODE_CHANGE_DETECTION_WIDTH; i++) {
        for (int j = 0; j < MODE_CHANGE_DETECTION_HEIGHT; j++) { // TODO: If the scan data comes in from the bottom, flip these conditions
            int value = getCircleVal(i, j, &circle);

            if (value) {
                rawScanArray[rc2iScan(j, i)] = value;
            }
        }
    }
}

void addRightTrigger() {
    static Circle circle =
            {SCAN_WIDTH - (MODE_CHANGE_DETECTION_WIDTH / 2),
             MODE_CHANGE_DETECTION_HEIGHT / 2,
             SCAN_WIDTH, SCAN_HEIGHT, 0, 0, LEFT, DOWN,
             MODE_CHANGE_DETECTION_HEIGHT / 2};

    for (int i = SCAN_WIDTH - MODE_CHANGE_DETECTION_WIDTH; i < SCAN_WIDTH; i++) {
        for (int j = 0; j < MODE_CHANGE_DETECTION_HEIGHT; j++) { // TODO: If the scan data comes in from the bottom, flip these conditions
            int value = getCircleVal(i, j, &circle);

            if (value) {
                rawScanArray[rc2iScan(j, i)] = value;
            }
        }
    }
}
#endif

