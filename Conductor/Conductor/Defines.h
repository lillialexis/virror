/*
 * Defines.h
 *
 * Created: 5/7/2018
 *  Author: Lilli Szafranski
 */


#ifndef DEFINES_H_
#define DEFINES_H_

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

#ifdef REAL_HARDWARE
#define DEFAULT_FRAME_RATE 60
#else
#define DEFAULT_FRAME_RATE 15
#endif
#define LOOP_TIME  (1000000 / DEFAULT_FRAME_RATE) /* In microseconds: 1,000,000 / FRAME_RATE */

#define FRAME_RATE_EMA_WEIGHT_TIME_FACTOR 300 /* Use the last 300 frames to calculate the average */


typedef __attribute__ ((__packed__)) struct {
	uint8_t g;
	uint8_t r;
	uint8_t b;
} RGB;

typedef struct {
	uint8_t h;
	uint8_t s;
	uint8_t v;
} HSV;

typedef struct {
	int16_t x;
	int16_t y;
} Coordinate;

typedef struct {
	uint8_t bass;
	uint8_t treble;
} Sound;

typedef struct {
	int time;
	int period;
	int rate;
} Clock;

typedef struct {
	int phase;
	int rise;
	int width;
	int fall;
	int count;
	int period;
	int rate;
} Wave;

//typedef struct {
//	uint8_t modeColor;
//	uint8_t modePattern;
//	uint8_t modeMotion;
//	uint8_t modeEffect;
//} EEpreset;



#endif /* DEFINES_H_ */
