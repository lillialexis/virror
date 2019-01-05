
// TODO: Probably convert to macros (saves programming space maybe?)
int rc2iScan(int row, int col) {
	return (row * SCAN_WIDTH) + col;
}

int rc2iLeds(int row, int col) {
	return (row * LED_WIDTH) + col;
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
//
//int round(float f) {
//    return (int)f;
//}
