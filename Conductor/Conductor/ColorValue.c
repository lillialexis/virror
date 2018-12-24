/*
 * ColorValue.c
 *
 * Created: 5/7/2018
 *  Author: Lilli Szafranski
 */

#include <math.h>
#include "Includes.h"

#define X_NUM_SECTIONS 4
#define Y_NUM_SECTIONS 3
#define NUM_COLORS 10
const uint8_t DISCO_COLORS[] = {
	COLOR_HSV_RED		, // W
	COLOR_HSV_SKY		, // C
	COLOR_HSV_VIOLET	, // C
	COLOR_HSV_YELLOW	, // W
	COLOR_HSV_GREEN		, // C
	COLOR_HSV_PINK		, // W
	COLOR_HSV_LIME_GREEN, // C
	COLOR_HSV_TEAL		, // C
	COLOR_HSV_ORANGE	, // W
	COLOR_HSV_BLUE		  // C
};

uint8_t discoFloor(Coordinate loc, int colorTime) {
	uint8_t X_SECTION_SIZE = WIDTH  / X_NUM_SECTIONS;
	uint8_t Y_SECTION_SIZE = HEIGHT / Y_NUM_SECTIONS;

	uint8_t X_SECTION_INDEX = MIN((uint8_t)((loc.x - X_MIN) / X_SECTION_SIZE), X_NUM_SECTIONS - 1); // MIN function used to fix edge case, where
	uint8_t Y_SECTION_INDEX = MIN((uint8_t)((loc.y - Y_MIN) / Y_SECTION_SIZE), Y_NUM_SECTIONS - 1); // the last pixel ends up in the next quanta

	uint8_t sectionIndex = (uint8_t)(X_SECTION_INDEX + (Y_SECTION_INDEX * X_NUM_SECTIONS));

	return DISCO_COLORS[(((colorTime / 32) + sectionIndex) % NUM_COLORS)];
}

#define NUMBER_OF_FLAMES      (4)
#define FLAME_WAVE_MIN        ((1 * (HEIGHT / 2)) + Y_MIN)
#define FLAME_WAVE_MAX_HEIGHT (Y_MAX - FLAME_WAVE_MIN)
#define FLAME_WAVE_WIDTH      (WIDTH / NUMBER_OF_FLAMES)
uint8_t fire(Coordinate loc, int colorTime) {

	double xLocAsRadians = (((double)(loc.x - X_MIN + ((double)colorTime * 7.0 /*+ 30.0*/)) / (double)WIDTH) * 2.0 * M_PI) * NUMBER_OF_FLAMES;
	double flameHeightTrigValAsPositiveDecimalLessThanOne = (sin(xLocAsRadians) + 1.0) / 2.0;
	double flameHeight                                    = ((double)FLAME_WAVE_MAX_HEIGHT * flameHeightTrigValAsPositiveDecimalLessThanOne) + (double)FLAME_WAVE_MIN;

	if (loc.y > flameHeight) return 0;

	double distBetweenHeightAndBottom = flameHeight - (double)Y_MIN;
	double percentageOfHeightPointIs  = (loc.y - (double)Y_MIN) / distBetweenHeightAndBottom;

	return MAX((uint8_t)(percentageOfHeightPointIs * COLOR_HSV_YELLOW), 1); // Don't want to return '0' here, so if '0', return '1'
}

//Coordinate origin = {0, 0};
RGB renderColorValue(uint8_t intensity, Coordinate loc, uint16_t ledNumber, RGB output)
{
	Clock     colorClock      = getColorClock();
	ColorMode colorMode       = getColorMode();
	uint8_t   globalIntensity = getGlobalIntensity();

	HSV tempHSV;
	tempHSV.s = 255;
	tempHSV.v = intensity;
	if		(colorMode == COLOR_RED					) { tempHSV.h = COLOR_HSV_RED			; }
	else if (colorMode == COLOR_ORANGE				) { tempHSV.h = COLOR_HSV_ORANGE		; }
	else if (colorMode == COLOR_YELLOW				) { tempHSV.h = COLOR_HSV_YELLOW		; }
	else if (colorMode == COLOR_GREEN				) { tempHSV.h = COLOR_HSV_GREEN			; }
	else if (colorMode == COLOR_TEAL				) { tempHSV.h = COLOR_HSV_TEAL			; }
	else if (colorMode == COLOR_SKY					) { tempHSV.h = COLOR_HSV_BLUE			; }
	else if (colorMode == COLOR_BLUE				) { tempHSV.h = COLOR_HSV_SKY			; }
	else if (colorMode == COLOR_VIOLET				) { tempHSV.h = COLOR_HSV_VIOLET		; }
	else if (colorMode == COLOR_PINK				) { tempHSV.h = COLOR_HSV_PINK			; }
	else if (colorMode == COLOR_WHITE				) { tempHSV.s = 0						; }
	else if (colorMode == COLOR_RAINBOW_SOLID		) { tempHSV.h =  colorClock.time % 255	; }
	else if (colorMode == COLOR_RAINBOW_LEFT		) { tempHSV.h = (colorClock.time % 255) + loc.x; }
	else if (colorMode == COLOR_RAINBOW_RIGHT		) { tempHSV.h = (colorClock.time % 255) - loc.x; }
	else if (colorMode == COLOR_RAINBOW_UP			) { tempHSV.h = (colorClock.time % 255) - loc.y; }
	else if (colorMode == COLOR_RAINBOW_DOWN		) { tempHSV.h = (colorClock.time % 255) + loc.y; }
	else if (colorMode == COLOR_RAINBOW_SPIN		) {
		loc = coordinateSpin(loc, colorClock, 0, FORWARD);
		tempHSV.h = (colorClock.time / 4) + loc.x + loc.y;
	}
	else if (colorMode == COLOR_COP_LIGHTS			) {
		if (colorClock.time % (colorClock.period / 20) > colorClock.period / 40)	{ tempHSV.h = (ledNumber < LED_DISPLAY_SIZE / 2) ? COLOR_HSV_RED : COLOR_HSV_BLUE; }
		else 																		{ tempHSV.h = (ledNumber < LED_DISPLAY_SIZE / 2) ? COLOR_HSV_BLUE : COLOR_HSV_RED; }
	}
	else if (colorMode == COLOR_FIRE				) {
		tempHSV.h = fire(loc, colorClock.time);

		if (tempHSV.h < COLOR_HSV_ORANGE) /* The closer we are to RED, the darker the LEDs should be. */
		tempHSV.v = (uint8_t)((double)tempHSV.v / ((1.0 - ((double)tempHSV.h / ((double)COLOR_HSV_ORANGE + 1.0))) * 4.0));
	}
	else if (colorMode == COLOR_DISCO				) {
		tempHSV.h = discoFloor(loc, colorClock.time);
	}
	else if (colorMode == COLOR_WAVES				) { }
	else if (colorMode == COLOR_CUSTOM_SOLID		) { tempHSV.h = customColor.h			; }
	else											  { tempHSV.h = COLOR_HSV_RED			; }
	
	output = hsvToRgb(tempHSV);//, output);
	output.g = ((unsigned int)output.g * globalIntensity) / 255;
	output.r = ((unsigned int)output.r * globalIntensity) / 255;
	output.b = ((unsigned int)output.b * globalIntensity) / 255;
	return(output);
}
