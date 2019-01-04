//
// Created by Lilli Szafranski on 2019-01-04.
//

#ifndef RAINBOWDISPLAYTEST_DEFINES_H
#define RAINBOWDISPLAYTEST_DEFINES_H

#define DEV_RIG
//#define REAL_RIG
//#define REAL_RIG_HALF


//#ifdef DEV_RIG
#define LED_WIDTH  16
#define LED_HEIGHT 16
//#endif

#ifdef REAL_RIG
#define LED_WIDTH  64
#define LED_HEIGHT 48
#endif

#ifdef REAL_RIG_HALF
#define LED_WIDTH  32
#define LED_HEIGHT 48
#endif


#define SENSOR_WIDTH  80
#define SENSOR_HEIGHT 14

//extern const float HORZ_SCALE_FACTOR ((float) LED_WIDTH / )


#endif //RAINBOWDISPLAYTEST_DEFINES_H
