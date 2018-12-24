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

#define LED_WIDTH      64
#define LED_HEIGHT     48
#define TOTAL_LEDS     (LED_WIDTH * LED_HEIGHT)

#define SCANNER_WIDTH  80
#define SCANNER_HEIGHT 14
#define SCANNER_MEASUREMENT_SIZE 4

#define NUMBER_OF_ROWS_WIRED_IN_SEQUENCE 6
#define NUMBER_OF_CHANNELS 8
#define NUMBER_OF_LEDS_PER_CHANNEL  (TOTAL_LEDS / 8)
#define TOTAL_NUMBER_OF_COLOR_CHARS (NUMBER_OF_LEDS_PER_CHANNEL * NUMBER_OF_CHANNELS * 6)

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
