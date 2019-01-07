

typedef enum {
    MODE_STATE_FADING_IN,
    MODE_STATE_RUNNING,
    MODE_STATE_FADING_OUT,
} ModeState;

void getFrame(int scanArray[], HSV ledArray[], int width, int height) {

    /* Initialize the background- and foreground-mode change counters. */
    static unsigned int backgroundModeTimeoutCounter = 0;
    static unsigned int foregroundModeTimeoutCounter = 0;

    static ModeState backgroundState = MODE_STATE_FADING_IN;
    static ModeState foregroundState = MODE_STATE_FADING_IN;

    static float backgroundAlpha = 1.0;
    static float foregroundAlpha = 1.0;

    static unsigned long globalFrame = 0UL;
    static unsigned int backgroundModeFrame = 0;
    static unsigned int foregroundModeFrame = 0;

    /* Get the scan. Did our scan detect a mode change? */
    int modeMask = scan(scanArray, LED_WIDTH, LED_HEIGHT);

    /* Is our background fading in? Fading out? Or just running normally? */
    if (backgroundState == MODE_STATE_FADING_IN) {
        /* If we're fading in... */

        if (backgroundModeTimeoutCounter >= MODE_CHANGE_FADE_DURATION) {
            backgroundState = MODE_STATE_RUNNING;
            backgroundModeTimeoutCounter = 0;
            backgroundAlpha = 1.0;

        } else {
            backgroundAlpha =
                    ((float)backgroundModeTimeoutCounter / (float) MODE_CHANGE_FADE_DURATION);
            backgroundModeTimeoutCounter++;

        }

    } else if (backgroundState == MODE_STATE_RUNNING) {
        /* If we're running... */

        /* If the scan detected a background-mode change or if our background-mode change counter hit
         * the threshold, change the background mode. */
        if (modeMask & BACKGROUND_MODE_CHANGE_FLAG ||
            backgroundModeTimeoutCounter >= MODE_CHANGE_DEFAULT_TIMEOUT) {

                backgroundState = MODE_STATE_FADING_OUT;
                backgroundModeTimeoutCounter = 0;

                if (modeMask & BACKGROUND_MODE_CHANGE_FLAG) {
                    addRipple({ACTIVE_RIPPLE, TODO_FIXME_RIPPLE_X_1,
                               TODO_FIXME_RIPPLE_Y, DEFAULT_RIPPLE_RADIUS,
                               DEFAULT_RIPPLE_WAVE_DURATION, DEFAULT_RIPPLE_TOTAL_DURATION,
                               DEFAULT_RIPPLE_CURRENT_FRAME, DEFAULT_RIPPLE_NUM_WAVES});
                }

        } else {
            backgroundModeTimeoutCounter++;

        }

    } else if (backgroundState == MODE_STATE_FADING_OUT) {
        /* If we're fading out... */

        if (backgroundModeTimeoutCounter >= MODE_CHANGE_FADE_DURATION) {
            setBackgroundModeVariants(getModeVariants());
            newBackgroundMode();

            backgroundState = MODE_STATE_FADING_IN;
            backgroundModeTimeoutCounter = 0;
            backgroundAlpha = 0.0;
            backgroundModeFrame = 0;

        } else {
            backgroundAlpha =
                    ((float)(MODE_CHANGE_FADE_DURATION - backgroundModeTimeoutCounter) /
                    (float) MODE_CHANGE_FADE_DURATION);
            backgroundModeTimeoutCounter++;

        }
    }

    /* Is our foreground fading in? Fading out? Or just running normally? */
    if (foregroundState == MODE_STATE_FADING_IN) {
        /* If we're fading in... */

        if (foregroundModeTimeoutCounter >= MODE_CHANGE_FADE_DURATION) {
            foregroundState = MODE_STATE_RUNNING;
            foregroundModeTimeoutCounter = 0;
            foregroundAlpha = 1.0;

        } else {
            foregroundAlpha =
                    ((float)foregroundModeTimeoutCounter / (float) MODE_CHANGE_FADE_DURATION);
            foregroundModeTimeoutCounter++;

        }

    } else if (foregroundState == MODE_STATE_RUNNING) {
        /* If we're running... */

        /* If the scan detected a foreground-mode change or if our foreground-mode change counter hit
         * the threshold, change the foreground mode. */
        if (modeMask & FOREGROUND_MODE_CHANGE_FLAG ||
            foregroundModeTimeoutCounter >= MODE_CHANGE_DEFAULT_TIMEOUT) {

                foregroundState = MODE_STATE_FADING_OUT;
                foregroundModeTimeoutCounter = 0;


                if (modeMask & FOREGROUND_MODE_CHANGE_FLAG) {
                    addRipple({ACTIVE_RIPPLE, TODO_FIXME_RIPPLE_X_2,
                               TODO_FIXME_RIPPLE_Y, DEFAULT_RIPPLE_RADIUS,
                               DEFAULT_RIPPLE_WAVE_DURATION, DEFAULT_RIPPLE_TOTAL_DURATION,
                               DEFAULT_RIPPLE_CURRENT_FRAME, DEFAULT_RIPPLE_NUM_WAVES});
                }

        } else {
            foregroundModeTimeoutCounter++;

        }

    } else if (foregroundState == MODE_STATE_FADING_OUT) {
        /* If we're fading out... */

        if (foregroundModeTimeoutCounter >= MODE_CHANGE_FADE_DURATION) {
            setForegroundModeVariants(getModeVariants());
            newForegroundMode();

            foregroundState = MODE_STATE_FADING_IN;
            foregroundModeTimeoutCounter = 0;
            foregroundAlpha = 0.0;
            foregroundModeFrame = 0;

        } else {
            foregroundAlpha =
                    ((float)(MODE_CHANGE_FADE_DURATION - foregroundModeTimeoutCounter) /
                    (float) MODE_CHANGE_FADE_DURATION);
            foregroundModeTimeoutCounter++;

        }
    }

//    if (globalFrame % 23 == 0) {
//        Serial.println("gf");
//        addRipple({ACTIVE_RIPPLE, globalFrame % 16,
//                   (globalFrame * 2) % 16, DEFAULT_RIPPLE_RADIUS,
//                   DEFAULT_RIPPLE_WAVE_DURATION, DEFAULT_RIPPLE_TOTAL_DURATION,
//                   DEFAULT_RIPPLE_CURRENT_FRAME, DEFAULT_RIPPLE_NUM_WAVES});
//    }

    /* Apply the loop's new background, then mix with the loop's new foreground. */
    applyBackground(ledArray, LED_WIDTH, LED_HEIGHT, backgroundAlpha, backgroundModeFrame);
    applyForeground(scanArray, ledArray, LED_WIDTH, LED_HEIGHT, foregroundAlpha, foregroundModeFrame);
    applyFeedback(ledArray, LED_WIDTH, LED_HEIGHT);

    globalFrame++;
    backgroundModeFrame++;
    foregroundModeFrame++;
}
