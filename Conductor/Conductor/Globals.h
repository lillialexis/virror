//
// Created by Lilli Szafranski on 5/7/18.
//

#ifndef VIRROR_CONDUCTOR_GLOBALS_H
#define VIRROR_CONDUCTOR_GLOBALS_H

#include <sys/types.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * Flow control * * * * * * * * * * * * * * * * * * * **/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define REAL_HARDWARE
#define REALLY_WRITING

#define SHOW_RENDER_TIMES
#define WITH_LESS_FREQUENCY
#define ONLY_WHEN_BAD

#ifdef WITH_LESS_FREQUENCY
#ifdef REALLY_WRITING
#define PRINT_FREQUENCY 20
#else
#define PRINT_FREQUENCY 100
#endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * Frame-rate/Loop stuff * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern u_long frameNumber; // TODO: Make this better controlled (i.e., only main can update it) global var
extern int    totalLoopDuration;
extern int    timeToExit;


#ifdef REAL_HARDWARE
#define DEFAULT_FRAME_RATE 60
#else
#define DEFAULT_FRAME_RATE 15
#endif
#define LOOP_TIME  (1000000 / DEFAULT_FRAME_RATE) /* In microseconds: 1,000,000 / FRAME_RATE */

#define FRAME_RATE_EMA_WEIGHT_TIME_FACTOR 300 /* Use the last 300 frames to calculate the average */

#define LED_WIDTH      64
#define LED_HEIGHT     48

#define SCANNER_WIDTH  80
#define SCANNER_HEIGHT 14
#define SCANNER_MEASUREMENT_SIZE 4

#endif //VIRROR_CONDUCTOR_GLOBALS_H
