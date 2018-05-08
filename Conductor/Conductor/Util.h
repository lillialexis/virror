//
// Created by Lilli Szafranski on 5/7/18.
//

#ifndef VIRROR_CONDUCTOR_UTIL_H
#define VIRROR_CONDUCTOR_UTIL_H

#include <unistd.h>
#include <objc/objc.h>
#include <stdlib.h>
#include <time.h>
#include "Globals.h"

//#define POSIX
//#ifdef POSIX
//#define RANDOM() (static BOOL here = NO; if (!here) srandom(time(NULL)); here = YES; random())
//#else
//#define RANDOM() arc4random()
//#endif
//#define RANDOM_INT(__MIN__, __MAX__) ((__MIN__) + RANDOM() % ((__MAX__+1) - (__MIN__)))


/* Shortcut for 2π, because fuck π */
#define M_2X_PI (M_PI + M_PI)

/* Convert latitudes and longitudes from ranges [0 : π] and [0 : 2π]/[0 : -2π] to [-π/2 : π/2] and [-π : π], respectively */
double slat(double l);
double slon(double l);

/* Convert latitudes and longitudes from ranges [-π/2 : π/2] and [-π : π] to [0 : π] and [0 : 2π], respectively */
double plat(double l);
double plon(double l);

#define DEGREES_TO_RADIANS(degrees) ((M_PI * degrees) / 180.0)
#define RADIANS_TO_DEGREES(radians) ((radians * 180.0) / M_PI)

/* Convert degrees value (0 - 360) to byte value (0 - 255) and percent value (0 - 100) to byte value (0 - 255) */
#define d2b(x) ((__uint8_t)(((double)x / 360.0) * 255.0))
#define p2b(x) ((__uint8_t)(((double)x / 100.0) * 255.0))

/* Convert byte value (0 - 255) to degrees value (0 - 360) and byte value (0 - 255) to percent value (0 - 100) */
#define b2d(x) ((int)(((double)x / 255.0) * 360.0))
#define b2p(x) ((int)(((double)x / 255.0) * 100.0))

/* Absolute value of a double and 1-dimensional, positive distance between two values */
#define absd(x) (x < 0 ? x * -1 : x)//(sqrt(x * x))
#define dist(x1, x2) (sqrt((x1 - x2) * (x1 - x2)))

/* Microseconds to seconds */
#define M2s(x) ((double)x / 1000000.0)

#define roundUpTo5(x) ((((int)(x) / 5) * 5) + 5)

int getRandom(int min, int max);
int getRandomCurveHigh(int min, int max);
int getRandomCurveLow(int min, int max);
int getRandomCurveMid(int min, int max);

typedef struct {
    __uint8_t r;
    __uint8_t g;
    __uint8_t b;
} RGB;

typedef struct {
    __uint8_t r;
    __uint8_t g;
    __uint8_t b;
    __uint8_t w;
} RGBW;

typedef struct {
    __uint8_t h;
    __uint8_t s;
    __uint8_t v;
} HSV;

RGB hsvToRgb (HSV hsv);
HSV rgbToHsv(RGB rgb);

int charsToUint16(char charArray[], uint16_t uintArray[], int numChars);

BOOL areEqualHsv(HSV hsv1, HSV hsv2);
BOOL areEqualRgb(RGB rgb1, RGB rgb2);

long currentTimeInMicroseconds();

double cos_lookup(double a);
double sin_lookup(double a);
double acos_lookup(double a);

double calculateEMA(double currentValue, long numberOfSamples, double lastEMA);

#endif //VIRROR_CONDUCTOR_UTIL_H
