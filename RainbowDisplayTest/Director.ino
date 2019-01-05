



void getFrame(int scanArray[], HSV ledArray[], int width, int height) {
    /* Initialize the background- and foreground-mode change counters. */
    static unsigned int backgroundModeChangeCounter = (MODE_CHANGE_COUNTER_TIMEOUT / 2);
    static unsigned int foregroundModeChangeCounter = 0;

    /* Get the scan. Did our scan detect a mode change? */
    int modeMask = scan(scanArray, LED_WIDTH, LED_HEIGHT);

    /* If the scan detected a background-mode change or if our background-mode change counter hit
     * the threshold, change the background mode. */
    if (modeMask & BACKGROUND_CHANGE ||
        backgroundModeChangeCounter == MODE_CHANGE_COUNTER_TIMEOUT) {
        newBackgroundMode();

        backgroundModeChangeCounter = 0;
    }

    /* If the scan detected a foreground-mode change or if our foreground-mode change counter hit
     * the threshold, change the foreground mode. */
    if (modeMask & FOREGROUND_CHANGE ||
        foregroundModeChangeCounter == MODE_CHANGE_COUNTER_TIMEOUT) {
        newForegroundMode();

        foregroundModeChangeCounter = 0;
    }

    /* Increase the background- and foreground-mode change counters. */
    foregroundModeChangeCounter++;
    backgroundModeChangeCounter++;

    /* Apply the loop's new background, then mix with the loop's new foreground. */
    applyBackground(ledArray, LED_WIDTH, LED_HEIGHT);
    applyForeground(scanArray, ledArray, LED_WIDTH, LED_HEIGHT);

}
