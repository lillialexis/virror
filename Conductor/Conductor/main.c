//
//  main.c
//  Conductor
//
//  Created by Lilli Szafranski on 5/7/18.
//  Copyright © 2018 Dao of Development. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include "Includes.h"
#include "Serial.h"

//#define SHOW_RENDER_TIMES

int serialDevice = -1;

long programStartTime   = 0;
long loopStartTime      = 0;
long idealLoopEndTime   = 0;
long timeAfterRender    = 0;
long drawStartTime      = 0;
int  lastDrawDuration   = 0;
int  lastRenderDuration = 0;
int  drawTimeCMA        = 0;
int  renderTimeCMA      = 0;

u_long frameNumber;

double globalBrightness = 0.75;

RGB LEDs[LED_WIDTH * LED_HEIGHT];
char scannerBuffer[SCANNER_WIDTH * SCANNER_HEIGHT * SCANNER_MEASUREMENT_SIZE];

//long currentTimeInMicroseconds()
//{
//    struct timeval time;
//    gettimeofday(&time, NULL);
//    long micros = (time.tv_sec * 1000000) + (time.tv_usec);
//
//    return micros;
//}

/* Delta in microseconds between the time value passed in and the time that the program launched (program time delta) */
int ptd(long someTimeVal)
{
    return (int) (someTimeVal - programStartTime);
}

/* Return 0 if successful, 1 otherwise. */
int updateLeds(u_long frameNumber)
{


    return 0;
}

uint8_t ColorCorrectLookup[256] = {0,	1,	1,	1,	2,	2,	2,	3,	3,	4,	4,	4,	5,	5,	5,	6,	6,	7,	7,	7,	8,	8,	8,	9,	9,	10,	10,	10,	11,	11,	12,	12,	13,	13,	13,	14,	14,	15,	15,	16,	16,	17,	17,	17,	18,	18,	19,	19,	20,	20,	21,	21,	22,	22,	23,	23,	24,	24,	25,	25,	26,	26,	27,	27,	28,	28,	29,	29,	30,	30,	31,	31,	32,	32,	33,	34,	34,	35,	35,	36,	37,	37,	38,	39,	39,	40,	41,	41,	42,	42,	43,	44,	44,	45,	46,	46,	47,	48,	48,	49,	50,	50,	51,	52,	53,	54,	55,	56,	57,	57,	58,	59,	60,	61,	62,	63,	64,	64,	65,	66,	67,	68,	69,	70,	71,	72,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	105,	106,	107,	108,	109,	110,	111,	112,	114,	115,	116,	117,	118,	119,	120,	121,	123,	124,	125,	126,	128,	129,	130,	131,	133,	134,	135,	137,	138,	139,	140,	142,	143,	144,	146,	147,	148,	149,	151,	152,	153,	155,	156,	158,	159,	161,	163,	164,	166,	167,	169,	171,	172,	174,	175,	177,	179,	180,	182,	183,	185,	187,	188,	190,	191,	193,	195,	197,	199,	201,	203,	205,	207,	209,	211,	213,	215,	217,	219,	221,	223,	225,	227,	229,	231,	233,	235,	237,	239,	241,	243,	245,	247,	249,	251,	253,	255};

#define RED_OFFSET   0
#define GREEN_OFFSET 8
#define BLUE_OFFSET  16

#ifdef REAL_HARDWARE

/* Return 0 if successful, 1 otherwise. */
int draw(Dodec *dodec, u_long frameNumber)
{
    int status = -1;
    char star = '*';

    status = write_data(serialDevice, &star, sizeof(star));
    if (status < 0) {
        printf("Error writing to serial device!\n");
        return 1;
    }

    int numberOfCharactersToSend = 181 * 16 * 6; /* 17376 */

    char readBuffer[numberOfCharactersToSend];
    char modifiedBuffer[50];

    memset(&readBuffer, '0', numberOfCharactersToSend);
    memset(&modifiedBuffer, '0', 50);

    /* define temp holder for RGB data */
    char grabr = 0;
    char grabg = 0;
    char grabb = 0;
    uint8_t sort, index, light, group, led_offset, group_offset = 0; /* vars used in routine */

    led_offset = 0;
    while (group_offset < 2)
    {
        index = sort = 0; /* start at lowest group offset and reset input buffer pointer */

        /* load the input buffer */
        while (sort < 24)
        {
            if (index < 12)
            { /* data only generated for 12 panels */
                __unsafe_unretained LED *led = quickLEDs[group_offset + index][led_offset];

                RGB rgb  = [led getCurrentRGB:frameNumber];

                readBuffer[sort++] = (char)(((double)ColorCorrectLookup[rgb.r]) * globalBrightness);
                readBuffer[sort++] = (char)(((double)ColorCorrectLookup[rgb.g]) * globalBrightness);
                readBuffer[sort++] = (char)(((double)ColorCorrectLookup[rgb.b]) * globalBrightness);

            }
            else
            { /* fill last two panels data with 0's */
                readBuffer[sort++] = 0;
                readBuffer[sort++] = 0;
                readBuffer[sort++] = 0;
            }

            index += 2;
        }

        led_offset++; /* move led offset for next loop */

        /* catch reaching the end of the first panel */
        if (led_offset == 181)
        {
            led_offset = 0;
            group_offset++;  /* when this reaches 2 the iterations has completed */
        }

        /* clear buffer */
        memset(&modifiedBuffer, 0, 24);

        sort = light = 0;
        while (light < 8)
        {
            grabr = readBuffer[sort++]; /* get red value   */
            grabg = readBuffer[sort++]; /* get green value */
            grabb = readBuffer[sort++]; /* get blue value  */

            index = 0;

            while (index < 8)
            {
                if (grabr & (1 << index)) {modifiedBuffer[RED_OFFSET + (7 - index)] |= (1 << light);}
                if (grabg & (1 << index)) {modifiedBuffer[GREEN_OFFSET + (7 - index)] |= (1 << light);}
                if (grabb & (1 << index)) {modifiedBuffer[BLUE_OFFSET + (7 - index)] |= (1 << light);}

                index++;
            }

            light++;
        }

        /* use the data */

        status = write_data(serialDevice, modifiedBuffer, 24);
        if (status < 0)
        {
            printf("Error writing to serial device!\n");
            return 1;
        }
    }

    return 0;
}

#else

/* Return 0 if successful, 1 otherwise. */
int draw(Dodec *dodec, u_long frameNumber)
{
    int status = -1;
    char star = '*';

    for (NSUInteger i = 0; i < dodec.NUM_BOARDS; i++) {
        for (NSUInteger j = 0; j < dodec.NUM_LEDS; j++) {
            NSUInteger index = (i * dodec.NUM_LEDS) + j;

            LED *led = [((Board *)[dodec.boards objectAtIndex:i]).leds objectAtIndex:j];

            dodec.buffer[index] = [led getCurrentRGB:frameNumber];
        }
    }

    /* Status should be equal to the number of bytes sent, or -1 if there was an error. */
    status = write_data(serialDevice, dodec.buffer, dodec.CURRENTLY_RENDERING_COUNT * sizeof(RGB));

    if (status < 0) {
        printf("Error writing to serial device!\n");
        return 1;
    }

    return 0;
}

#endif

/* Return 0 if successful, 1 otherwise. */
int loop(Dodec *dodec)
{
    int status = 0;

    while (1) {

        /* Capture the start time of our current loop, and calculate the time when this loop would ideally end,
         * which is the current start time + the ideal loop duration (in MS) - however long our last draw function took. */
        loopStartTime = currentTimeInMicroseconds();
        idealLoopEndTime = loopStartTime + LOOP_TIME - lastDrawDuration;

#ifdef SHOW_RENDER_TIMES
        printf("Looping.\n\t\tStart time: %lu (%i Ms after program start time)\n\t\tIdeal duration for this loop:" \
                        "%li Ms\n\t\tIdeal time to begin next draw: %lu (%i Ms after program start time)\n",
               loopStartTime, ptd(loopStartTime), (long)(LOOP_TIME - lastDrawDuration), idealLoopEndTime, ptd(idealLoopEndTime));
#endif

        status = updateLeds(dodec, frameNumber);
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

        drawStartTime = currentTimeInMicroseconds();

        status = draw(dodec, frameNumber);
        if (status != 0) {
            return 1;
        }

        lastDrawDuration = (int) (currentTimeInMicroseconds() - drawStartTime);

#ifdef SHOW_RENDER_TIMES
        DLog(@"");

        printf("Draw duration: %i\n", lastDrawDuration);
//#endif

        renderTimeCMA = (int)(((long)lastRenderDuration + ((frameNumber - 1) * (long)renderTimeCMA)) / (frameNumber));
        drawTimeCMA   = (int)(((long)lastDrawDuration   + ((frameNumber - 1) * (long)drawTimeCMA))   / (frameNumber));

        if (lastDrawDuration + (timeAfterRender - loopStartTime) > LOOP_TIME)
            printf("FRAME TAKING TOO LONG!! Render time: %i Ms (%.4f s), draw time: %i Ms (%.4f s), effective frame rate: ~%.2f fps\n",
                    lastRenderDuration, M2s(lastRenderDuration), lastDrawDuration, M2s(lastDrawDuration),
                    (1.0 / M2s((lastDrawDuration + (timeAfterRender - loopStartTime)))));
            //printf("DRAWING TAKING TOO LONG!! (%i Ms)\n", lastDrawDuration);

        printf("Average render duration: %i (%.4f s), ", renderTimeCMA, M2s(renderTimeCMA));
        printf("average draw duration: %i (%.4f s)\n", drawTimeCMA, M2s(drawTimeCMA));
#endif

        frameNumber++;
    }
}

int main(int argc, const char * argv[]) {

    programStartTime = currentTimeInMicroseconds();

#ifdef SHOW_RENDER_TIMES
    printf("Program starting at: %lu\n", programStartTime);
#endif

    Dodec *dodec = [Dodec dodec];

    /* Just to cut down on the time it takes to go through the array LED during draw, I'm short-cutting their pointer addresses to a quick c-lookup-array in this file */
    for (Board *board in dodec.boards)
    for (LED *led in board.leds)
    quickLEDs[board.index][led.index] = led;


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
    int status = loop(dodec);

    return status;

}
