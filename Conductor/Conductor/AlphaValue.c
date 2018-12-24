/*
 * AlphaValue.c
 *
 * Created: 5/7/2018
 *  Author: Lilli Szafranski
 */


#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "Includes.h"

int fadeCalcSingle(int startValue, int endValue, int time, int period)
{
	//NOW <= START + (FADE_CT * (END - START))/FADE_L
	return startValue + time * ((double)endValue - startValue) / period;
}

uint8_t waveRender(uint8_t wave, int position) //returns intensity for led based on alpha pattern
{
	int localClock = (position + waveSets[wave].count + waveSets[wave].phase) % waveSets[wave].period; //adjust for phase and position
	// 	int		 localClock = position + alphaClock.time - waveSets[wave].phase; //get clock, shrink to size and offset
	// 	while   (localClock < 0)					 {localClock %= alphaClock.period;} ; //fix any phase wrapping
	if		(localClock <= waveSets[wave].rise)  {return(fadeCalcSingle(0, 255, localClock,  waveSets[wave].rise));}
	else if (localClock <= waveSets[wave].width) {return (255);}
	else if (localClock <= waveSets[wave].fall)  {return(fadeCalcSingle(255, 0, localClock - waveSets[wave].width,  waveSets[wave].fall - waveSets[wave].width));}
	else										 {return (0);}
}

uint8_t wink(Coordinate loc, uint8_t waveSet)
{
	Coordinate center = findCenter(loc);
	return(waveRender(waveSet, findRadius(loc, center)));
}

uint8_t segmentFromCenter(Coordinate loc, Coordinate center, int ledIndex)
{
	int16_t rise = loc.y - center.y;
	int16_t run  = loc.x - center.x;

	int16_t slope = rise / (run == 0 ? 1 : run);

	if (rise >= 0 && run >= 0) return /*1;*/(slope <   1 ? 0 : 1); /* Quadrant I   */
	if (rise >= 0 && run <= 0) return /*2;*/(slope <= -1 ? 2 : 3); /* Quadrant II  */
	if (rise <= 0 && run <= 0) return /*3;*/(slope <   1 ? 4 : 5); /* Quadrant III */
	if (rise <= 0 && run >= 0) return /*4;*/(slope <= -1 ? 6 : 7); /* Quadrant IV  */
	
	return 0;
}

uint8_t spiral(Coordinate loc, uint16_t ledIndex, AlphaMode mode)
{
	//Coordinate origin = {0, 0};

	Coordinate center = findCenter(loc);
	int radius = findRadius(loc, center);

	if (radius < 5) return 255;

	// TODO: Update this stuff once when changing into a new alpha pattern mode and again when exiting;
	//		 not every time this method is called for every frame for every LED
	//alphaClock.period = 255;
	//alphaClock.rate = 30; //rate is controlled by motion menu


	Direction direction;
	#ifdef BLINKY_TIT_0V2
	if (ledIndex < LED_DISPLAY_SIZE / 2) {
		if (mode == ALPHA_SPIRAL_1) direction = FORWARD;
		//		if (alphaClock.rate > 0) direction = FORWARD;
		else                          direction = BACKWARD;
		} else {
		if (mode == ALPHA_SPIRAL_1) direction = BACKWARD;
		//		if (alphaClock.rate > 0) direction = BACKWARD;
		else                          direction = FORWARD;
	}
	#else
	if (mode == ALPHA_SPIRAL_1)  direction = FORWARD;
	//	if (alphaClock.rate > 0) direction = FORWARD;
	else                         direction = BACKWARD;
	#endif

	Coordinate shifted = {loc.x - center.x, loc.y - center.y};
	Coordinate spun    = coordinateSpin(shifted, getAlphaClock(), radius*2, direction);
	Coordinate spunUnshifted = {spun.x + center.x, spun.y + center.y};

	uint8_t octant = segmentFromCenter(spunUnshifted, center, ledIndex);

	if (octant % 2 == 0) return 255;
	else                 return 0;
}

typedef struct {
	uint8_t ledIndex;
	uint8_t twinkleTime;
	uint8_t counter;
} Sparkle;

BOOL isSparkleInitialized = false;//FALSE;

#define SPARKLE_MIN 20
#define SPARKLE_MAX 255
#define BACKGROUND_VARIANCE 20

#define NUMBER_OF_SPARKLES LED_DISPLAY_SIZE / SPARKLE_DENSITY
Sparkle sparkles[NUMBER_OF_SPARKLES];

#define MIN_TWINKLE_TIME			64
#define MAX_TWINKLE_TIME			256
#define POTENTIAL_TWINKLE_FRAMES	(MAX_TWINKLE_TIME - MIN_TWINKLE_TIME)

uint8_t fadeInOutSparkle(uint8_t lowValue, uint8_t highValue, uint16_t twinkleTime, uint16_t count)
{
	uint16_t rise  = twinkleTime / 2;//3 * twinkleTime / 6;
	uint16_t fall  = twinkleTime / 2;//1 * twinkleTime / 6;

	if		(count <= rise)  {return(fadeCalcSingle(lowValue, highValue, count, rise));}
	else                     {return(fadeCalcSingle(highValue, lowValue, count - rise, fall));}
}

void resetSparkle(uint8_t sparkleIndex)
{
	static uint32_t seed = 0;
	//srand(seed++);

	uint8_t newSparkleIndex = rand() % LED_DISPLAY_SIZE;
	uint8_t twinkleTime = (rand() % POTENTIAL_TWINKLE_FRAMES) + MIN_TWINKLE_TIME;
	
	//Sparkle newSparkle = {newSparkleIndex, twinkleTime, twinkleTime};

	sparkles[sparkleIndex].ledIndex = newSparkleIndex;
	sparkles[sparkleIndex].twinkleTime = twinkleTime;
	sparkles[sparkleIndex].counter = twinkleTime;
}

uint8_t sparkle(Coordinate loc, uint8_t ledIndex, uint8_t sparkleSpeed, uint8_t backgroundSpeed)
{
	if (!isSparkleInitialized) {

		memset(sparkles, NULL, NUMBER_OF_SPARKLES * sizeof(Sparkle));

		for (uint8_t i = 0; i < NUMBER_OF_SPARKLES; i++) {
			resetSparkle(i);
		}
		
		isSparkleInitialized = false;//TRUE;
	}
	
	int8_t sparkleIndex = -1; /* Start off with -1, to find if this LED is a sparkle (and if we have duplicate sparkles). */
	for (uint8_t i = 0; i < NUMBER_OF_SPARKLES; i++) { /* Loop through the array, and see if our sparkle is in it. */
		if (sparkles[i].ledIndex == ledIndex && sparkleIndex == -1) { /* If this is the first time we found our LED. */
			sparkleIndex = i;

			if (sparkles[i].counter < (getAlphaClock().rate / 2)) {
				resetSparkle(i);
			} else {
				sparkles[i].counter -= (getAlphaClock().rate / 2);
			}
		} else if (sparkles[i].ledIndex == ledIndex && sparkleIndex != -1) { /* If the LED ends up in here twice... */
			resetSparkle(i);
		}
	}
	
	static unsigned int backgroundCount = 0;
	uint16_t backgroundTwinkleTime = ((ledIndex % 11 + ledIndex % 7 + ledIndex % 3) + 1) * (256 / (getAlphaClock().rate / 2));//(1000 / backgroundSpeed); /* Kind of make it random seeming... */
	
	if (ledIndex == 0) backgroundCount++;
	//if (backgroundCount > 65500) backgroundCount = 0;
	
	if (sparkleIndex != -1) return fadeInOutSparkle(SPARKLE_MIN, SPARKLE_MAX, sparkles[sparkleIndex].twinkleTime, (sparkles[sparkleIndex].twinkleTime - sparkles[sparkleIndex].counter));
	else                    return fadeInOutSparkle(SPARKLE_MIN, SPARKLE_MIN + BACKGROUND_VARIANCE, backgroundTwinkleTime, backgroundCount % backgroundTwinkleTime);
}

#define THROB_CLOCK_LENGTH			(30.0)
#define FIRST_BEAT_HIGH_TIMESTAMP	(THROB_CLOCK_LENGTH / 4.0)
#define FIRST_BEAT_LOW_TIMESTAMP	(FIRST_BEAT_HIGH_TIMESTAMP	+ (THROB_CLOCK_LENGTH / 6.0))
#define SECOND_BEAT_HIGH_TIMESTAMP	(FIRST_BEAT_LOW_TIMESTAMP	+ (THROB_CLOCK_LENGTH / 6.0))
#define SECOND_BEAT_LOW_TIMESTAMP	(SECOND_BEAT_HIGH_TIMESTAMP	+ (THROB_CLOCK_LENGTH / 4.0))
//#define SECOND_BEAT_LOW_TIMESTAMP	(THROB_CLOCK_LENGTH)//(SECOND_BEAT_HIGH_TIMESTAMP	+ (THROB_CLOCK_LENGTH / 4.0))

#define THROB_MIN_VAL						(20)
#define THROB_VARIANCE						(255 - THROB_MIN_VAL)
#define THROB_BETWEEN_BEATS_MIN_PERCENTAGE	(0.8)
uint8_t throb(Coordinate loc)
{
	static long throbCounter = 0;
	int         throbClock   = 0;

	if (throbCounter == THROB_CLOCK_LENGTH * LED_DISPLAY_SIZE) throbCounter = 0;

	throbClock = (throbCounter++ / LED_DISPLAY_SIZE);

	double percentageThroughThrobPhase = 0;

	if 		(throbClock < FIRST_BEAT_HIGH_TIMESTAMP)	percentageThroughThrobPhase = ((double)throbClock / (FIRST_BEAT_HIGH_TIMESTAMP - 0.0));
	else if (throbClock < FIRST_BEAT_LOW_TIMESTAMP)		percentageThroughThrobPhase = (1.0 - ((((double)throbClock - FIRST_BEAT_HIGH_TIMESTAMP) / (FIRST_BEAT_LOW_TIMESTAMP - FIRST_BEAT_HIGH_TIMESTAMP)) * THROB_BETWEEN_BEATS_MIN_PERCENTAGE));
	else if (throbClock < SECOND_BEAT_HIGH_TIMESTAMP)	percentageThroughThrobPhase = (((((double)throbClock - FIRST_BEAT_LOW_TIMESTAMP) / (SECOND_BEAT_HIGH_TIMESTAMP - FIRST_BEAT_LOW_TIMESTAMP)) * THROB_BETWEEN_BEATS_MIN_PERCENTAGE) + (1.0 - THROB_BETWEEN_BEATS_MIN_PERCENTAGE));
	else if (throbClock < SECOND_BEAT_LOW_TIMESTAMP)	percentageThroughThrobPhase = (1.0 - (((double)throbClock - SECOND_BEAT_HIGH_TIMESTAMP) / (SECOND_BEAT_LOW_TIMESTAMP - SECOND_BEAT_HIGH_TIMESTAMP)));

	double percentageAsRadians = percentageThroughThrobPhase * 0.5 * M_PI; /* We want the sine value between 0 and π/2 (varies from 0 to 1) */
	double percentageAsSineVal = sin(percentageAsRadians);

//	double percentageAsGoodInputForLog = (1.0 - percentageThroughThrobPhase) * 10.0;
//	double logValueAsPercentage        = (1.0 + (-1.0 * log10(percentageAsGoodInputForLog))) / 10.0;

//	return (uint8_t)((logValueAsPercentage * THROB_VARIANCE) + THROB_MIN_VAL);
	return (uint8_t)((percentageAsSineVal * THROB_VARIANCE) + THROB_MIN_VAL);
}

uint8_t renderAlphaValue(Coordinate loc, uint16_t ledNumber)
{
	AlphaMode alphaMode = getAlphaMode();

//	if		(alphaMode == ALPHA_WINK_1		) { return wink(loc, ALPHA_WINK_1); }
//	else
	if 		(alphaMode == ALPHA_WINK_2		) { return wink(loc, ALPHA_WINK_2); }
//	else if (alphaMode == ALPHA_WINK_3		) { return wink(loc, ALPHA_WINK_3); }
//	else if (alphaMode == ALPHA_WINK_4		) { return wink(loc, ALPHA_WINK_4); }
	else if (alphaMode == ALPHA_SOUND_1		) { return soundResults.bass; }
	else if (alphaMode == ALPHA_SOUND_2		) { return soundResults.treble; }
	else if (alphaMode == ALPHA_SOUND_3		) {
		if (ledNumber > LED_DISPLAY_SIZE / 2) { return soundResults.bass; }
		else                                  { return soundResults.treble; }
	}
	else if (alphaMode == ALPHA_SPIRAL_1	) { return spiral(loc, ledNumber, ALPHA_SPIRAL_1); }
	else if (alphaMode == ALPHA_SPIRAL_2	) { return spiral(loc, ledNumber, ALPHA_SPIRAL_2); }
//	else if (alphaMode == ALPHA_CROSS_2		) { return 0x15; }
//	else if (alphaMode == ALPHA_CROSS_3		) { return 0x15; }
//	else if (alphaMode == ALPHA_CROSS_4		) { return 0x15; }
	else if (alphaMode == ALPHA_SPARKLE_1	) { return sparkle(loc, ledNumber, 1, 1); }
//	else if (alphaMode == ALPHA_SPARKLE_2	) { return sparkle(loc, ledNumber, 3, 3); }
//	else if (alphaMode == ALPHA_SPARKLE_3	) { return sparkle(loc, ledNumber, 9, 9); }
//	else if (alphaMode == ALPHA_FIREWORKS	) { return 0x15; }
//	else if (alphaMode == ALPHA_WAVES		) { return 0x15; }
	else if (alphaMode == ALPHA_THROB		) { return throb(loc); }
	else if (alphaMode == ALPHA_SOLID		) { return 0xff; }
	
	return 0xff;
}
