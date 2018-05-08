/*
 * Math.c
 *
 * Created: 5/7/2018
 *  Author: Lilli Szafranski
 */

#include <math.h>
#include "Includes.h"

/*const int SQR_TABLE[] PROGMEM = {0, 0x1, 0x4, 0x9, 0x10, 0x19, 0x24, 0x31, 0x40, 0x51, 0x64, 0x79, 0x90, 0xA9, 0xC4, 0xE1, 0x100, 0x121, 0x144, 0x169, 0x190, 0x1B9, 0x1E4, 0x211, 0x240, 0x271, 0x2A4, 0x2D9, 0x310, 0x349, 0x384, 0x3C1, 0x400, 0x441, 0x484, 0x4C9, 0x510, 0x559, 0x5A4, 0x5F1, 0x640, 0x691, 0x6E4, 0x739, 0x790, 0x7E9, 0x844, 0x8A1, 0x900, 0x961, 0x9C4, 0xA29, 0xA90, 0xAF9, 0xB64, 0xBD1, 0xC40, 0xCB1, 0xD24, 0xD99, 0xE10, 0xE89, 0xF04, 0xF81, 0x1000, 0x1081, 0x1104, 0x1189, 0x1210, 0x1299, 0x1324, 0x13B1, 0x1440, 0x14D1, 0x1564, 0x15F9, 0x1690, 0x1729, 0x17C4, 0x1861, 0x1900, 0x19A1, 0x1A44, 0x1AE9, 0x1B90, 0x1C39, 0x1CE4, 0x1D91, 0x1E40, 0x1EF1, 0x1FA4, 0x2059, 0x2110, 0x21C9, 0x2284, 0x2341, 0x2400, 0x24C1, 0x2584, 0x2649, 0x2710, 0x27D9, 0x28A4, 0x2971, 0x2A40, 0x2B11, 0x2BE4, 0x2CB9, 0x2D90, 0x2E69, 0x2F44, 0x3021, 0x3100, 0x31E1, 0x32C4, 0x33A9, 0x3490, 0x3579, 0x3664, 0x3751, 0x3840, 0x3931, 0x3A24, 0x3B19, 0x3C10, 0x3D09, 0x3E04, 0x3F01, 0x4000, 0x4101, 0x4204, 0x4309, 0x4410, 0x4519, 0x4624, 0x4731, 0x4840, 0x4951, 0x4A64, 0x4B79, 0x4C90, 0x4DA9, 0x4EC4, 0x4FE1, 0x5100, 0x5221, 0x5344, 0x5469, 0x5590, 0x56B9, 0x57E4, 0x5911, 0x5A40, 0x5B71, 0x5CA4, 0x5DD9, 0x5F10, 0x6049, 0x6184, 0x62C1, 0x6400, 0x6541, 0x6684, 0x67C9, 0x6910, 0x6A59, 0x6BA4, 0x6CF1, 0x6E40, 0x6F91, 0x70E4, 0x7239, 0x7390, 0x74E9, 0x7644, 0x77A1, 0x7900, 0x7A61, 0x7BC4, 0x7D29, 0x7E90, 0x7FF9, 0x8164, 0x82D1, 0x8440, 0x85B1, 0x8724, 0x8899, 0x8A10, 0x8B89, 0x8D04, 0x8E81, 0x9000, 0x9181, 0x9304, 0x9489, 0x9610, 0x9799, 0x9924, 0x9AB1, 0x9C40, 0x9DD1, 0x9F64, 0xA0F9, 0xA290, 0xA429, 0xA5C4, 0xA761, 0xA900, 0xAAA1, 0xAC44, 0xADE9, 0xAF90, 0xB139, 0xB2E4, 0xB491, 0xB640, 0xB7F1, 0xB9A4, 0xBB59, 0xBD10, 0xBEC9, 0xC084, 0xC241, 0xC400, 0xC5C1, 0xC784, 0xC949, 0xCB10, 0xCCD9, 0xCEA4, 0xD071, 0xD240, 0xD411, 0xD5E4, 0xD7B9, 0xD990, 0xDB69, 0xDD44, 0xDF21, 0xE100, 0xE2E1, 0xE4C4, 0xE6A9, 0xE890, 0xEA79, 0xEC64, 0xEE51, 0xF040, 0xF231, 0xF424, 0xF619, 0xF810, 0xFA09, 0xFC04, 0xFE01 };
uint8_t squareRoot (unsigned int value)
{
	uint8_t guess = 0;
	uint8_t mask = 0x80;
	unsigned int check ;
	for (uint8_t x = 0; x < 8; x++)
	{
		guess |= mask; //set the bit
		check = pgm_read_word(&SQR_TABLE[guess]);		
		if (value < check)
		{
			guess ^= mask;	//remove the set bit
		}
		mask = (mask >> 1); //shift the mask		
	}
	return guess;
}*/

int findRadius(Coordinate A, Coordinate B)
{
  	unsigned int run = abs(B.x - A.x);
  	unsigned int rise = abs(B.y - A.y);
	  
 	return sqrt(run * run + rise * rise);
//	return squareRoot(run * run + rise * rise);
	
// 	if		((rise >> 1) > run) { return rise;                     } // rise is much larger
// 	else if ((run >> 1) > rise)	{ return run;                      } // rise is much larger
// 	else						{ return ((rise >> 1)+(run >> 1)); } // rise is much larger
}

// uint8_t findCenterX(Coordinate loc);
// #ifdef BLINKY_TIT_0V2
// {
// 	if (loc.x < X_MID)		{return(X_C1);
// 	else					{return(X_C2);
// }
// #else
// {
// 	return(X_MID);
// }
// #endif
// uint8_t findCenterY(Coordinate loc);
// #ifdef BLINKY_TIT_0V2
// {
// 	if (loc.x < X_MID)		{return(Y_C1);
// 	else					{return(Y_C2);
// }
// #else
// {
// 	return(Y_MID);
// }
// #endif

Coordinate findCenter(Coordinate loc) {
#ifdef BLINKY_TIT_0V2

	if (loc.x < X_MID)		{loc.x = X_C1; loc.y = Y_C1;}
	else					{loc.x = X_C2; loc.y = Y_C2;}
	return loc;

#else

	loc.x = X_MID; loc.y = Y_MID; //{X_MID, Y_MID};
	return loc;

#endif
}

//RGB hsvToRgb (HSV hsv, RGB rgb)
//{
//	//RGB rgb;
//	unsigned char region, p, q, t;
//	unsigned int h, s, v, remainder;
//
//	if (hsv.s == 0)
//	{
//		rgb.r = hsv.v;
//		rgb.g = hsv.v;
//		rgb.b = hsv.v;
//
//		return rgb;
//	}
//
//	// converting to 16 bit to prevent overflow
//	h = hsv.h;
//	s = hsv.s;
//	v = hsv.v;
//
//	region = h / 43;
//	remainder = (h - (region * 43)) * 6;
//
//	p = (v * (255 - s)) >> 8;
//	q = (v * (255 - ((s * remainder) >> 8))) >> 8;
//	t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;
//
//	switch (region)
//	{
//		case 0:
//			rgb.r = v;
//			rgb.g = t;
//			rgb.b = p;
//			break;
//
//		case 1:
//			rgb.r = q;
//			rgb.g = v;
//			rgb.b = p;
//			break;
//
//		case 2:
//			rgb.r = p;
//			rgb.g = v;
//			rgb.b = t;
//			break;
//
//		case 3:
//			rgb.r = p;
//			rgb.g = q;
//			rgb.b = v;
//			break;
//
//		case 4:
//			rgb.r = t;
//			rgb.g = p;
//			rgb.b = v;
//			break;
//
//		default:
//			rgb.r = v;
//			rgb.g = p;
//			rgb.b = q;
//			break;
//	}
//
//	return rgb;
//}


// Hold sine table in ram for fastest access
const uint8_t SINX[] = {0, 6, 13, 19, 25, 31, 38, 44, 50, 56, 62, 68, 74, 80, 86, 92, 98, 104, 109, 115, 121, 126, 132, 137, 142, 147, 152, 157, 162, 167, 172, 176, 181, 185, 190, 194, 198, 202, 205, 209, 213, 216, 219, 222, 225, 228, 231, 234, 236, 238, 241, 243, 244, 246, 248, 249, 250, 251, 252, 253, 254, 254, 255, 255, 255};
int sineTable(uint8_t input)
{
	uint8_t arc = (input >> 6) ;// Div 64;  //determine phase of wave form
	if      (arc == 0) { return SINX[input];		}
	else if (arc == 1) { return SINX[128-input];	}
	else if (arc == 2) { return -SINX[input-128];	}
	else if (arc == 3) { return -SINX[256-input];	}
    
    return 0;
}

int cosineTable(uint8_t input)
{
	return sineTable(input+64);
}

Coordinate coordinateResonance (Coordinate loc)
{
	Coordinate newLoc;
	newLoc.x = cosineTable(loc.x) - sineTable(loc.y);
	newLoc.y = sineTable(loc.x) + cosineTable(loc.y);
	return newLoc;
}

Coordinate coordinateSpin (Coordinate loc, Clock clock, uint8_t drag, Direction direction)
{
	Coordinate newLoc;
	uint16_t adjustedTime = (clock.time - drag < 0) ? clock.time + clock.period - drag : clock.time - drag;

	if (direction == BACKWARD) adjustedTime = clock.period - adjustedTime;

	uint8_t angle = adjustedTime / 4;
	newLoc.x = (( int)loc.x * cosineTable(angle) / 255) - (( int)loc.y * sineTable(angle) / 255);
	newLoc.y = (( int)loc.x * sineTable(angle) / 255) + (( int)loc.y * cosineTable(angle) / 255);
	return newLoc;
}

// Coordinate routines
#ifdef CARTESIAN
uint8_t xPosition(uint8_t ledNumber)
{
	return X_OFFSET[ledNumber];//pgm_read_byte(&X_OFFSET[ledNumber]);
}
uint8_t yPosition(uint8_t ledNumber)
{
	return Y_OFFSET[ledNumber];//pgm_read_byte(&Y_OFFSET[ledNumber]);
}
#endif

#ifdef LINEAR
uint8_t xPosition(uint8_t ledNumber)
{
	return ledNumber;
}
uint8_t yPosition(uint8_t ledNumber)
{
	return 1;
}
#endif
