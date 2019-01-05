
// TODO: Probably convert to macros (saves programming space maybe?)
int rc2iScan(int row, int col) {
	return (row * SCAN_WIDTH) + col;
}

int rc2iLeds(int row, int col) {
	return (row * LED_WIDTH) + col;
}

