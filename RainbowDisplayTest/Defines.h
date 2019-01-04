//
// Created by Lilli Szafranski on 2019-01-04.
//

#ifndef RAINBOWDISPLAYTEST_DEFINES_H
#define RAINBOWDISPLAYTEST_DEFINES_H

#define DEV_RIG
//#define REAL_RIG
//#define REAL_RIG_HALF

#define DEV_SCAN
//#define REAL_SCAN

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

#define SCAN_WIDTH  80
#define SCAN_HEIGHT 14

#define FOREGROUND_CHANGE 1
#define BACKGROUND_CHANGE 2

#define CYCLE_TIME 2500

//extern const float HORZ_SCALE_FACTOR ((float) LED_WIDTH / )


#endif //RAINBOWDISPLAYTEST_DEFINES_H
