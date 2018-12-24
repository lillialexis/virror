/*
 * Hardware.h
 *
 * Created: 5/7/2018
 *  Author: Lilli Szafranski
 */


#ifndef HARDWARE_H_
#define HARDWARE_H_


/* * * * * SELECT DISPLAY TYPE * * * * */
#define WS_DEV
//#define VIRROR
/* * * * * * * * * * * * * * * * * * * */

#ifdef WS_DEV
extern const uint8_t Y_OFFSET[];
extern const uint8_t X_OFFSET[];
#define CARTESIAN

#define LED_WIDTH  16
#define LED_HEIGHT 16
#define TOTAL_LEDS (LED_WIDTH * LED_HEIGHT)

#define LED_DISPLAY_SIZE    TOTAL_LEDS
#define X_MIN               8
#define X_MAX               248
#define Y_MIN               8
#define Y_MAX               248
#define SPARKLE_DENSITY     10
#endif

#ifdef VIRROR
#define CARTESIAN

#define LED_WIDTH  64
#define LED_HEIGHT 48
#define TOTAL_LEDS (LED_WIDTH * LED_HEIGHT)

extern const uint8_t X_OFFSET[];
extern const uint8_t Y_OFFSET[];
#define LED_DISPLAY_SIZE    TOTAL_LEDS
#define X_MIN               30 // TODO: Fill these in
#define X_MAX               224
#define Y_MIN               38
#define Y_MAX               109
#define X_C1                75
#define Y_C1                70
#define X_C2                180
#define Y_C2                70
#define SPARKLE_DENSITY     5
#endif

#define WIDTH               (X_MAX - X_MIN)
#define HEIGHT              (Y_MAX - Y_MIN)
#define X_MID               ((X_MAX - X_MIN) / 2 + X_MIN)
#define Y_MID               ((Y_MAX - Y_MIN) / 2 + Y_MIN)

/* * * * * * * * SELECT OTHER HARDWARE TYPE * * * * * * * * */
#define OPTION1
//#define OPTION2
/* * * * * * * * * * * * * * * * * * * * * * * * * */


#ifdef OPTION1
#define SCANNER_WIDTH  80
#define SCANNER_HEIGHT 14
#define SCANNER_MEASUREMENT_SIZE 4

#define NUMBER_OF_ROWS_WIRED_IN_SEQUENCE 6
#define NUMBER_OF_CHANNELS 8
#define NUMBER_OF_LEDS_PER_CHANNEL  (TOTAL_LEDS / 8)
#define TOTAL_NUMBER_OF_COLOR_CHARS (NUMBER_OF_LEDS_PER_CHANNEL * NUMBER_OF_CHANNELS * 6)
#endif

#ifdef OPTION2
// TODO: Put other hardware consts here
#endif

#endif /* HARDWARE_H_ */
