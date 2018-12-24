/*
 * Hardware.h
 *
 * Created: 5/7/2018
 *  Author: Lilli Szafranski
 */


#ifndef HARDWARE_H_
#define HARDWARE_H_

//#import <cstdint>

/* * * * * SELECT DISPLAY TYPE * * * * */
#define WS_DEV
//#define VIRROR
/* * * * * * * * * * * * * * * * * * * */

#ifdef WS_DEV
extern const uint8_t Y_OFFSET[];
extern const uint8_t X_OFFSET[];
#define CARTESIAN
#define LED_DISPLAY_SIZE    256
#define WS_REPEAT           1
#define FRAME_RATE          25
#define X_MIN               8
#define X_MAX               248
#define Y_MIN               8
#define Y_MAX               248
#define SPARKLE_DENSITY     10
#endif

#ifdef VIRROR
#define CARTESIAN
extern const uint8_t X_OFFSET[];
extern const uint8_t Y_OFFSET[];
#define LED_DISPLAY_SIZE    144
#define WS_REPEAT           1
#define FRAME_RATE          30  // fps
#define X_MIN               30
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
// TODO: Put other hardware consts here
#endif

#ifdef OPTION2
// TODO: Put other hardware consts here
#endif

#endif /* HARDWARE_H_ */
