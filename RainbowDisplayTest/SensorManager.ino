
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

///* If we hit any errors in this code, set the scaled scan array to be entirely 255, so it might
// * be obvious when looking at the screen. */
//void setError(int scaledScanArray[], int width, int height) {
//    memset(scaledScanArray, 255, width * height * sizeof(int));
//}

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
#ifdef RESET_MODE_CHANGE_TRIGGERS_ON_POSITIVE
        positiveCounterFgd = 0;
        negativeCounterFgd = 0;
#else
        /* Since our mode change triggers are ignored during fade transitions, keep returning
         * true until the negativeCounter hits its threshold, _then_ reset our counters. */
#endif

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
#ifdef RESET_MODE_CHANGE_TRIGGERS_ON_POSITIVE
        positiveCounterBgd = 0;
        negativeCounterBgd = 0;
#else
        /* Since our mode change triggers are ignored during fade transitions, keep returning
         * true until the negativeCounter hits its threshold, _then_ reset our counters. */
#endif

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
    // TODO: Test this shit for reallllllllllz

    /* This method assumes that the horizontal scan data is always equal to or larger than the
     * LED panels, and the vertical scan data is always less than or equal to the LED panels. */

    if (SCAN_WIDTH < width || SCAN_HEIGHT > height) { /* Error state for now */
        /* If we hit any errors in this code, set the scaled scan array to be entirely 255, so it might
         * be obvious when looking at the screen. */
        memset(scaledScanArray, 255, width * height * sizeof(int));

        return;
    }

    /* Here for dev purposes... */
    if (SCAN_WIDTH == width && SCAN_HEIGHT == height) {
        for (int i = 0; i < width * height; i++) {
            scaledScanArray[i] = rawScanArray[i];
        }

        return;
    }

    /* https://docs.google.com/spreadsheets/d/1hgkGhmtR7GcLJbPbDvgXvMd3qLdtR0yp0fW34N3dFMc/edit?usp=sharing */
    float v_ScaleFactor = (float)SCAN_HEIGHT / (float)height;
    float h_ScaleFactor = (float)SCAN_WIDTH / (float)width;

    const unsigned int kernel_offset = 2; /* Is that what it's called? */
    const unsigned int kernel_width  = (kernel_offset * 2) + 1;

    unsigned int kernel[kernel_width];

    memset(&kernel, 0, kernel_width * sizeof(int));

    for (unsigned int x = 0; x < width; x++) {

        unsigned int scaled_x     = ((float)x * h_ScaleFactor);
        unsigned int scan_x_left  = max(0, (scaled_x - kernel_width));
        unsigned int scan_x_right = min((scaled_x + kernel_width), (SCAN_WIDTH - 1));

        unsigned int x_delta = scan_x_right - scan_x_left + 1;

        for (unsigned int y = 0; y < height; y++) {

            unsigned int scaled_y =
                    (y < height / 2) ?
                            (float)y * v_ScaleFactor :
                            min((SCAN_HEIGHT - 1), (((float)y * v_ScaleFactor) + v_ScaleFactor));

            /* I can probably read out of the raw scan array directly, but I don't feel like
             * thinking about how it's set up right now. */
            memset(&kernel, 0, kernel_width * sizeof(int));
            for (unsigned int i = 0; i < x_delta; i++) {
                kernel[i] = rawScanArray[rc2iScan(scaled_y, (scan_x_left + i))];
            }

            scaledScanArray[rc2iLed(y, x)] = sampled(kernel, kernel_width, scaled_x, x_delta);
        }
    }
}

int sampled(unsigned int kernel[], int kernelWidth, int xPos, int xDelta) {
    float percent = 1.0 / kernelWidth;

//    float sum = 0.0;
//    for (unsigned int i = 0; i < xDelta; i++) {
//
//        sum =
//    }

    // TODO: I give up...
}

int scan(int scaledScanArray[], int width, int height) {
#ifdef USING_EMPTY_SCAN_DATA
    return NO_MODE_CHANGE;
#endif

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
