

// TODO: Possibly convert to macros? (Saves programming space/execution time or not?)
int rc2iScan(int row, int col) {
	return (row * SCAN_WIDTH) + col;
}

int rc2iLeds(int row, int col) {
	return (row * LED_WIDTH) + col;
}

float dist(float x1, float y1, float x2, float y2) {
    return sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}

unsigned int xy2i(unsigned int x, unsigned int y) {
    bool LorR;
    y = LED_HEIGHT - y - 1; // invert display

    if (y % 16 < 8) // 2 * 8 strands run back and fourth
    { LorR = true; } // line goes LtoR
    else { LorR = false; } // line goes RtoL

    unsigned int channel_index = (y % 8) * ledsPerPin; // array index for start of the LED strand of the LED in question
    unsigned int strand_index = (y / 8) * LED_WIDTH;

    if (LorR) { return channel_index + strand_index + x; }
    else { return channel_index + strand_index + LED_WIDTH - x - 1; }
}
