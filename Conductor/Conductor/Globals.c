
#include <string.h>
#include "Globals.h"

//int menuClock 		= 0;
//int screenClock		= 0;
//int bluetoothClock	= 0	;

//RGB ledArray [LED_DISPLAY_SIZE];

#define COLOR_CLOCK_SIZE 0x3ff
Clock alphaClock = {0, 256, 1};
Clock colorClock = {0, COLOR_CLOCK_SIZE, 5}; // creates 4 255 loops for spin

inline Clock getAlphaClock() {
    return alphaClock;
}

inline Clock getColorClock() {
    return colorClock;
}

// Clock routines
Clock clockUpdate(Clock clock)
{
    clock.time += clock.rate; // update
    if		(clock.time > clock.period)		{ clock.time -= clock.period; }
    else if (clock.time < 0)				{ clock.time += clock.period; }
    return clock;
}

void updateClocks() {
    colorClock = clockUpdate(colorClock);
    alphaClock = clockUpdate(alphaClock);
}

void waveUpdate(uint8_t wave)
{
    waveSets[wave].count += waveSets[wave].rate;
    if		(waveSets[wave].count > waveSets[wave].period)	{ waveSets[wave].count -= waveSets[wave].period; }
    else if (waveSets[wave].count < 0)						{ waveSets[wave].count += waveSets[wave].period; }
}

void updateWaves() {
    for (uint8_t i = 0; i < WAVE_SET_SIZE; i++)	{
        waveUpdate(i);
    }
}

///*	phase, rise, width, fall, count, period, rate */
Wave waveSets [WAVE_SET_SIZE] = {	//{0,5,15,20,1,32,1},		// WINK_1
        {0,15,30,45,1,96,2},		// WINK_2
        //{0,5,20,25,1,48,3},		// WINK_3
        //{0,20,40,60,1,128,5},		// WINK_4
        //{0,10,20,30,1,50,1},		// SOUND_1
        //{0,10,20,30,1,50,1},		// SOUND_2
        //{0,10,20,30,1,50,1},		// SOUND_3
        //{0,10,20,30,1,50,1},		// SPIRAL_1
        //{0,10,20,30,1,50,1},		// SPIRAL_2
        //{0,10,20,30,1,50,1},		// CROSS_2
        //{0,10,20,30,1,50,1},		// CROSS_3
        //{0,10,20,30,1,50,1},		// CROSS_4
        //{0,10,20,30,1,50,1},		// SPARKLE_1
        //{0,10,20,30,1,50,1},		// SPARKLE_2
        //{0,10,20,30,1,50,1},		// SPARKLE_3
        //{0,10,20,30,1,50,1},		// FIREWORKS
        //{0,10,20,30,1,50,1},		// WAVES
        //{0,10,20,30,1,50,1},		// SOLID
};


HSV customColor;


uint8_t    effectFlags;	// bit flags defines what effects are used
ColorMode  colorMode;	// number defines what menu mode is used and the render mode
AlphaMode  alphaMode;	// number defines what menu mode is used and the render mode
MotionMode motionMode;	// number defines what menu mode is used and the render mode
uint8_t    brightness;	// saves the mode for menu operation
uint8_t    loadPos = MENU_START;
uint8_t    savePos = MENU_START;
uint8_t    soundBuffer[SOUND_BUFFER_SIZE];
uint8_t    soundPointer;
Sound      soundResults;
Sound      soundGain;
Sound      soundDecayLoops;

//uint8_t EEMEM colorPreset[10];
//uint8_t EEMEM alphaPreset[10];
//uint8_t EEMEM motionPreset[10];
//uint8_t EEMEM effectPreset[10];
//uint8_t EEMEM brightSave;
//uint8_t EEMEM lastPreset;
//uint8_t EEMEM safePreset;

const char printBufferLine1[PRINT_BUFFER_SIZE + 1];
const char printBufferLine2[PRINT_BUFFER_SIZE + 1];
void clearPrintBuffer(char *printBuffer) {
    memset(printBuffer, 0, PRINT_BUFFER_SIZE + 1);
}


inline ColorMode   getColorMode()                   { return colorMode; }
inline void        setColorMode(ColorMode mode)     { colorMode = mode; }

inline AlphaMode   getAlphaMode()                   { return alphaMode; }
inline void        setAlphaMode(AlphaMode mode)     { alphaMode = mode; }

inline uint8_t getGlobalIntensity()
{
    return (uint8_t)(((int)brightness * brightness) / 40 + 4); // scale from 100 to 255
}

//void saveBrightness()
//{
//    eeprom_write_byte(&brightSave, brightness);
//}
//
//void loadBrightness()
//{
//    brightness = eeprom_read_byte(&brightSave);
//    if (brightness == 0) {
//        setBrightness(BRIGHT_MINIMUM);
//    } else if (brightness < BRIGHT_MINIMUM || brightness > BRIGHT_MAXIMUM) {
//        setBrightness(BRIGHT_DEFAULT);
//    } // contain erroneous values
//}

inline uint8_t getBrightness()
{
    return brightness;
}

inline void setBrightness(uint8_t mode)
{
    brightness = mode;
    //saveBrightness();
}

void hackMotion(int moveRate)
{
    for (uint8_t x  = 0; x < WAVE_SET_SIZE; x++)
    {
        waveSets[x].rate = moveRate;
    }
    alphaClock.rate = moveRate;
}

void setMotionMode(MotionMode mode)
{
    if		(mode == MOTION_BACKWARD_6	)	{ hackMotion(-64	); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_BACKWARD_5	)	{ hackMotion(-32	); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_BACKWARD_4	)	{ hackMotion(-16	); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_BACKWARD_3	)	{ hackMotion(-8		); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_BACKWARD_2	)	{ hackMotion(-4		); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_BACKWARD_1	)	{ hackMotion(-2		); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_STILL		)	{ hackMotion( 0		); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_FORWARD_1	)	{ hackMotion( 2		); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_FORWARD_2	)	{ hackMotion( 4		); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_FORWARD_3	)	{ hackMotion( 8		); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_FORWARD_4	)	{ hackMotion( 16	); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_FORWARD_5	)	{ hackMotion( 32	); } // needs a sub-menu to seek through static position
    else if (mode == MOTION_FORWARD_6	)	{ hackMotion( 64	); } // needs a sub-menu to seek through static position

    motionMode = mode;
}

inline MotionMode getMotionMode()
{
    return motionMode;
}

inline void setEffectFlags(uint8_t flag)
{
    if (effectFlags & (1<<flag)) {	// if flag is set
        effectFlags &= ~(1<<flag);	// clear the flag
    } else {
        effectFlags |= (1<<flag);	// set the flag
    }
}

inline uint8_t getEffectFlags()
{
    return effectFlags;
}

//inline uint8_t getLoadPos() { return loadPos; }
//inline void    setLoadPos(uint8_t pos)
//{
//    loadPos = pos;
//    //loadPreset(pos);
//}
//
//inline uint8_t getSavePos() { return savePos; }
//inline void    setSavePos(uint8_t pos)
//{
//    savePos = pos;
//    //savePreset(pos);
//}

//void savePreset (uint8_t num)
//{
//    eeprom_write_byte(&colorPreset[num], getColorMode());
//    _delay_us(50);
//    eeprom_write_byte(&alphaPreset[num], getAlphaMode());
//    _delay_us(50);
//    eeprom_write_byte(&motionPreset[num], getMotionMode());
//    _delay_us(50);
//    eeprom_write_byte(&effectPreset[num], effectFlags);
//    _delay_us(50);
//    eeprom_write_byte(&lastPreset, num);
//}
//
//void loadPreset (uint8_t num)
//{
////#define TESTING
//#ifdef TESTING
//    setColorMode(COLOR_FIRE);
//	setAlphaMode(ALPHA_SOLID);
//	setMotionMode(MOTION_FORWARD_6);
//	effectFlags	= EFFECT_COLOR;
//#else
//    setColorMode((ColorMode)eeprom_read_byte(&colorPreset[num]));
//    setAlphaMode((AlphaMode)eeprom_read_byte(&alphaPreset[num]));
//    setMotionMode((MotionMode)eeprom_read_byte(&motionPreset[num]));
//    effectFlags	= eeprom_read_byte(&effectPreset[num]);
//    _delay_us(50);
//    eeprom_write_byte(&lastPreset, num);
//#endif
//}

// Save routines EEPROM
//void presetDefaults()
//{
//    colorMode  = COLOR_PINK;
//    alphaMode  = ALPHA_WINK_2;
//    motionMode = MOTION_FORWARD_2;
//    savePreset(0);
//    colorMode  = COLOR_RAINBOW_SPIN;
//    alphaMode  = ALPHA_SOLID;
//    motionMode = MOTION_STILL;
//    savePreset(1);
//    colorMode  = COLOR_TEAL;
//    alphaMode  = ALPHA_WINK_2;
//    motionMode = MOTION_FORWARD_1;
//    savePreset(2);
//    colorMode  = COLOR_RAINBOW_SOLID;
//    alphaMode  = ALPHA_SPARKLE_1;
//    motionMode = MOTION_FORWARD_2;
//    savePreset(3);
//    colorMode  = COLOR_WHITE;
//    alphaMode  = ALPHA_SPIRAL_1;
//    motionMode = MOTION_FORWARD_4;
//    savePreset(4);
//    colorMode  = COLOR_SKY;
//    alphaMode  = ALPHA_SPARKLE_1;
//    motionMode = MOTION_FORWARD_2;
//    savePreset(5);
//    colorMode  = COLOR_RAINBOW_UP;
//    alphaMode  = ALPHA_SPIRAL_1;
//    motionMode = MOTION_FORWARD_5;
//    savePreset(6);
//    colorMode  = COLOR_YELLOW;
//    alphaMode  = ALPHA_SPIRAL_2;
//    motionMode = MOTION_FORWARD_3;
//    savePreset(7);
//    colorMode  = COLOR_RAINBOW_SPIN;
//    alphaMode  = ALPHA_SOUND_1;
//    motionMode = MOTION_FORWARD_1;
//    savePreset(8);
//    colorMode  = COLOR_RAINBOW_SOLID;
//    alphaMode  = ALPHA_SOUND_3;
//    motionMode = MOTION_FORWARD_1;
//    savePreset(9);
//    eeprom_write_byte(&lastPreset, 0);
//    _delay_us(50);
//    eeprom_write_byte(&safePreset, EEPROM_SAFE);
//    lcdClearScreen();
//    lcdPrintln_P(PSTR("Default presets"));
//    lcdPrint_P(PSTR("loaded"));
//    _delay_ms(1000);
//    lcdClearScreen();
//}
//
//void checkPresetDefaults ()
//{
//    uint8_t check = eeprom_read_byte(&safePreset);
//    if (check != EEPROM_SAFE) {presetDefaults();}
//}

