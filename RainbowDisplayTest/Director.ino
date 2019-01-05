

typedef enum {
    MODE_STATE_FADING_IN,
    MODE_STATE_RUNNING,
    MODE_STATE_FADING_OUT,
} ModeState;

void getFrame(int scanArray[], HSV ledArray[], int width, int height) {
    /* Initialize the background- and foreground-mode change counters. */
    static unsigned int backgroundModeChangeCounter = (MODE_CHANGE_COUNTER_TIMEOUT / 2);
    static unsigned int foregroundModeChangeCounter = 0;

    static ModeState backgroundState = MODE_STATE_FADING_IN;
    static ModeState foregroundState = MODE_STATE_FADING_IN;

    static float backgroundAlpha = 1.0;
    static float foregroundAlpha = 1.0;

    /* Get the scan. Did our scan detect a mode change? */
    int modeMask = scan(scanArray, LED_WIDTH, LED_HEIGHT);

    /* If the scan detected a background-mode change or if our background-mode change counter hit
     * the threshold, change the background mode. */
    if (modeMask & BACKGROUND_MODE_CHANGE_FLAG ||
        backgroundModeChangeCounter == MODE_CHANGE_COUNTER_TIMEOUT) {
        newBackgroundMode();

        backgroundModeChangeCounter = 0;
    }

    /* If the scan detected a foreground-mode change or if our foreground-mode change counter hit
     * the threshold, change the foreground mode. */
    if (modeMask & FOREGROUND_MODE_CHANGE_FLAG ||
        foregroundModeChangeCounter == MODE_CHANGE_COUNTER_TIMEOUT) {
        newForegroundMode();

        foregroundModeChangeCounter = 0;
    }

    /* Increase the background- and foreground-mode change counters. */
    foregroundModeChangeCounter++;
    backgroundModeChangeCounter++;

    /* Apply the loop's new background, then mix with the loop's new foreground. */
    applyBackground(ledArray, LED_WIDTH, LED_HEIGHT, backgroundAlpha);
    applyForeground(scanArray, ledArray, LED_WIDTH, LED_HEIGHT, foregroundAlpha);
}
