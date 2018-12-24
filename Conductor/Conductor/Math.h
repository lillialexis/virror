/*
 * Math.h
 *
 * Created: 5/7/2018
 *  Author: Lilli Szafranski
 */


#ifndef MATH_H_
#define MATH_H_

//uint8_t squareRoot (unsigned int value);
int findRadius(Coordinate A, Coordinate B);

Coordinate findCenter(Coordinate loc);
//RGB hsvToRgb (HSV hsv, RGB rgb);

int sineTable(uint8_t input);
int cosineTable(uint8_t input);

Coordinate coordinateResonance (Coordinate loc);
Coordinate coordinateSpin (Coordinate loc, Clock clock, uint8_t drag, Direction direction);

uint8_t xPosition(uint8_t ledNumber);
uint8_t yPosition(uint8_t ledNumber);

#define MIN(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })
#define MAX(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#endif /* MATH_H_ */
