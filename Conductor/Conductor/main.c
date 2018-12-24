//
//  main.c
//  Conductor
//
//  Created by Lilli Szafranski on 5/7/18.
//  Copyright © 2018 Dao of Development. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include "Includes.h"
#include "Serial.h"

//#define SHOW_RENDER_TIMES

int serialDevice = -1;

long programStartTime     = 0;
long loopStartTime        = 0;
long idealLoopEndTime     = 0;
long timeAfterRender      = 0;
long doSerialStartTime    = 0;
int  lastDoSerialDuration = 0;
int  lastRenderDuration   = 0;
int  doSerialTimeCMA      = 0;
int  renderTimeCMA        = 0;

u_long frameNumber;

double globalBrightness = 0.75;

RGB ledArray[TOTAL_LEDS];
char scannerInputBuffer[SCANNER_WIDTH * SCANNER_HEIGHT * SCANNER_MEASUREMENT_SIZE];
uint16_t scannerValues[SCANNER_WIDTH * SCANNER_HEIGHT];

/* Delta in microseconds between the time value passed in and the time that the program launched (program time delta) */
int ptd(long someTimeVal) {
    return (int) (someTimeVal - programStartTime);
}

/* Return 0 if successful, 1 otherwise. */
int updateLeds(u_long frameNumber) {
    renderFrame(ledArray, frameNumber);
    addScannerValues(ledArray, scannerValues);

    return 0;
}

uint8_t ColorCorrectLookup[256] = {0,	1,	1,	1,	2,	2,	2,	3,	3,	4,	4,	4,	5,	5,	5,	6,	6,	7,	7,	7,	8,	8,	8,	9,	9,	10,	10,	10,	11,	11,	12,	12,	13,	13,	13,	14,	14,	15,	15,	16,	16,	17,	17,	17,	18,	18,	19,	19,	20,	20,	21,	21,	22,	22,	23,	23,	24,	24,	25,	25,	26,	26,	27,	27,	28,	28,	29,	29,	30,	30,	31,	31,	32,	32,	33,	34,	34,	35,	35,	36,	37,	37,	38,	39,	39,	40,	41,	41,	42,	42,	43,	44,	44,	45,	46,	46,	47,	48,	48,	49,	50,	50,	51,	52,	53,	54,	55,	56,	57,	57,	58,	59,	60,	61,	62,	63,	64,	64,	65,	66,	67,	68,	69,	70,	71,	72,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	105,	106,	107,	108,	109,	110,	111,	112,	114,	115,	116,	117,	118,	119,	120,	121,	123,	124,	125,	126,	128,	129,	130,	131,	133,	134,	135,	137,	138,	139,	140,	142,	143,	144,	146,	147,	148,	149,	151,	152,	153,	155,	156,	158,	159,	161,	163,	164,	166,	167,	169,	171,	172,	174,	175,	177,	179,	180,	182,	183,	185,	187,	188,	190,	191,	193,	195,	197,	199,	201,	203,	205,	207,	209,	211,	213,	215,	217,	219,	221,	223,	225,	227,	229,	231,	233,	235,	237,	239,	241,	243,	245,	247,	249,	251,	253,	255};

#define RED_OFFSET   0
#define GREEN_OFFSET 8
#define BLUE_OFFSET  16

#ifdef REAL_HARDWARE

/* Return 0 if successful, 1 otherwise. */
int doSerial(u_long frameNumber) {
    int status = -1;
    char star = '*';

    /* Write a star */
    status = write_data(serialDevice, &star, sizeof(star));
    if (status < 0) {
        return 1;
    }

    // TODO: Jesse, swizzle code goes here!!

    /* Send LEDs */
    status = write_data(serialDevice, ledArray, TOTAL_LEDS * sizeof(RGB));
    if (status < 0) {
        return 1;
    }

    char result;

    /* Read a star */
    status = read_data(serialDevice, &result, sizeof(result));
    if (status < 0) {
        return status;
    }

    if (result != '*') {
        printf("Unexpected read result\n");
    }

    /* Read scanner values */
    status = read_data(serialDevice, scannerInputBuffer, SCANNER_WIDTH * SCANNER_HEIGHT * SCANNER_MEASUREMENT_SIZE * sizeof(char));
    if (status < 0) {
        return status;
    }

    /* Convert scanner chars to bytes */
    status = charsToUint16(scannerInputBuffer, scannerValues, SCANNER_WIDTH * SCANNER_HEIGHT * SCANNER_MEASUREMENT_SIZE);
    if (status < 0) {
        return status;
    }

    return 0;
}

#else

/* Return 0 if successful, 1 otherwise. */
int doSerial(u_long frameNumber)
{
    int status = -1;

    /* Status should be equal to the number of bytes sent, or -1 if there was an error. */
    status = write_data(serialDevice, ledArray, TOTAL_LEDS * sizeof(RGB));
    if (status < 0) {
        return 1;
    }

    return 0;
}

#endif

/* Return 0 if successful, 1 otherwise. */
int loop() {
    int status = 0;

    while (1) {

        /* Capture the start time of our current loop, and calculate the time when this loop would ideally end,
         * which is the current start time + the ideal loop duration (in MS) - however long our last doSerial function took. */
        loopStartTime = currentTimeInMicroseconds();
        idealLoopEndTime = loopStartTime + LOOP_TIME - lastDoSerialDuration;

#ifdef SHOW_RENDER_TIMES
        printf("Looping.\n\t\tStart time: %lu (%i Ms after program start time)\n\t\tIdeal duration for this loop:" \
                        "%li Ms\n\t\tIdeal time to begin next doSerial: %lu (%i Ms after program start time)\n",
                loopStartTime, ptd(loopStartTime), (long)(LOOP_TIME - lastDoSerialDuration), idealLoopEndTime, ptd(idealLoopEndTime));
#endif

        status = updateLeds(frameNumber);
        if (status != 0) {
            return 1;
        }

        /* Now capture the current time, and see if our render function took up the whole duration of our loop. */
        timeAfterRender    = currentTimeInMicroseconds();
        lastRenderDuration = (int)(timeAfterRender - loopStartTime);

#ifdef SHOW_RENDER_TIMES
        printf("Render duration: %li\n", timeAfterRender - loopStartTime);
#endif

        /* If it took up the whole duration of our loop, that's bad. Print this. */
        //if (timeAfterRender >= idealLoopEndTime)
        //   printf("RENDERING TAKING TOO LONG!! (%li Ms, %.4f s)\n", lastRenderDuration, ((double)(lastRenderDuration)/1000000.0));
//#endif

        useconds_t sleepTime = (useconds_t) (idealLoopEndTime > timeAfterRender ? idealLoopEndTime - timeAfterRender : 0);

#ifdef SHOW_RENDER_TIMES
        printf("Sleep duration: %u\n", sleepTime);
#endif

#ifdef REALLY_WRITING
        usleep(sleepTime);
#endif

        doSerialStartTime = currentTimeInMicroseconds();

        status = doSerial(frameNumber);
        if (status != 0) {
            return 1;
        }

        lastDoSerialDuration = (int) (currentTimeInMicroseconds() - doSerialStartTime);

#ifdef SHOW_RENDER_TIMES

        printf("DoSerial duration: %i\n", lastDoSerialDuration);
//#endif

        renderTimeCMA   = (int)(((long)lastRenderDuration   + ((frameNumber - 1) * (long)renderTimeCMA))   / (frameNumber));
        doSerialTimeCMA = (int)(((long)lastDoSerialDuration + ((frameNumber - 1) * (long)doSerialTimeCMA)) / (frameNumber));

        if (lastDoSerialDuration + (timeAfterRender - loopStartTime) > LOOP_TIME)
            printf("FRAME TAKING TOO LONG!! Render time: %i Ms (%.4f s), doSerial time: %i Ms (%.4f s), effective frame rate: ~%.2f fps\n",
                    lastRenderDuration, M2s(lastRenderDuration), lastDoSerialDuration, M2s(lastDoSerialDuration),
                    (1.0 / M2s((lastDoSerialDuration + (timeAfterRender - loopStartTime)))));
        //printf("SERIAL TAKING TOO LONG!! (%i Ms)\n", lastDoSerialDuration);

        printf("Average render duration: %i (%.4f s), ", renderTimeCMA, M2s(renderTimeCMA));
        printf("average doSerial duration: %i (%.4f s)\n", doSerialTimeCMA, M2s(doSerialTimeCMA));
#endif

        frameNumber++;
    }
}

void setUp() {
    memset(&ledArray,           0, TOTAL_LEDS * sizeof(RGB));
    memset(&scannerInputBuffer, 0, SCANNER_WIDTH * SCANNER_HEIGHT * SCANNER_MEASUREMENT_SIZE * sizeof(char));
    memset(&scannerValues,      0, SCANNER_WIDTH * SCANNER_HEIGHT * sizeof(uint16_t));
}

int main(int argc, const char * argv[]) {

    programStartTime = currentTimeInMicroseconds();

#ifdef SHOW_RENDER_TIMES
    printf("Program starting at: %lu\n", programStartTime);
#endif

    setUp();

#ifdef REAL_HARDWARE
#ifdef NOT_RASPI
    serialDevice = new_serial_device("/dev/cu.usbmodem1395821");
#else
    serialDevice = new_serial_device("/dev/ttyACM0");
#endif
#else
    serialDevice = new_serial_device("/dev/ttyS0");
#endif

    if (serialDevice < 0) {
        printf("Error creating serial device!\n");
        return 1;
    }

    /*static u_long*/ frameNumber = 1;

    /* Should only exit on error... */
    int status = loop();

    return status;

}
