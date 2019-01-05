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

#ifdef DEV_RIG
#define LED_WIDTH  16
#define LED_HEIGHT 16
#endif

#ifdef REAL_RIG
#define LED_WIDTH  64
#define LED_HEIGHT 48
#endif

#ifdef REAL_RIG_HALF
#define LED_WIDTH  32
#define LED_HEIGHT 48
#endif

#define SCAN_WIDTH  16//80
#define SCAN_HEIGHT 16//14

//#define MODE_CHANGE_COUNTER_TIMEOUT 100000
#define MODE_CHANGE_COUNTER_TIMEOUT 20000

#define FOREGROUND_CHANGE 1
#define BACKGROUND_CHANGE 2

typedef struct {
    unsigned int h;
    unsigned int s;
    unsigned int l;
} HSL;

typedef struct {
    unsigned int r;
    unsigned int g;
    unsigned int b;
} RGB;

typedef struct {
    unsigned int h;
    unsigned int s;
    unsigned int v;
} HSV;

#define DEFAULT_SATURATION 255
#define DEFAULT_VALUE      255

#define BYTE_MAX 255

typedef enum {
    LEFT,
    RIGHT
} xDirection;

typedef enum {
    UP,
    DOWN
} yDirection;

typedef struct {
    float x;// = 0;
    float y;// = 0;
    float width;
    float height;
    float rise;// = LED_HEIGHT;
    float run;// = LED_WIDTH;
    xDirection xDir;// = LEFT;
    yDirection yDir;// = DOWN;
    float radius;// = LED_WIDTH;
} Circle;




#endif //RAINBOWDISPLAYTEST_DEFINES_H
