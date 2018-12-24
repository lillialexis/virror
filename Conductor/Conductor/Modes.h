/*
 * Modes.h
 *
 * Created: 5/7/2018
 *  Author: Lilli Szafranski
 */


#ifndef MODES_H_
#define MODES_H_

enum menuDefinitions
{
    MENU_START				= 0,
    MENU_TITLE				= 253,
    MENU_SIZE				= 254,
    MENU_EXIT				= 255,
    MENU_VALUE_TIMEOUT		= 400, // number of MENU_LOOP_DELAY before timeout
    MENU_AUTO_INCREMENT		= 85, //ms
    MENU_AUTO_DELAY_LOOPS	= 3, //times the auto increment delays fully before auto inc, delay first start
    MENU_LOOP_DELAY			= 30 / 10, //seconds the menu delays before timeout && number of milliseconds delayed between menu loops
    //MENU_TIMEOUT			= MENU_LOOP_DELAY * FRAME_RATE, // seconds times frame rate to get render count before timeout
    EEPROM_SAFE				= 0XA5
};

typedef enum
{
    BUTTON_NONE,
    BUTTON_RIGHT,
    BUTTON_LEFT,
    BUTTON_CLICK
} ButtonState;

typedef enum
{
    MAIN_ALPHA				= MENU_START,
    MAIN_COLOR							,
    MAIN_MOTION							,
//	MAIN_EFFECT							,
    MAIN_BRIGHTNESS						,
    MAIN_SAVE							,
    MAIN_LOAD							,
    MAIN_COUNT							, // Always equals the count of the previous list of menu items, so long as MENU_START = 0
    MAIN_TITLE				= MENU_TITLE,
    MAIN_SIZE				= MENU_SIZE
} MainMenuMode;

typedef enum
{
    //	ALPHA_WINK_1			= MENU_START,
    ALPHA_WINK_2			= MENU_START,
    //	ALPHA_WINK_3						,
    //	ALPHA_WINK_4						,
    ALPHA_SOUND_1						,
    ALPHA_SOUND_2						,
    ALPHA_SOUND_3						,
    ALPHA_SPIRAL_1						,
    ALPHA_SPIRAL_2						,
    // 	ALPHA_CROSS_2						,
    // 	ALPHA_CROSS_3						,
    // 	ALPHA_CROSS_4						,
    ALPHA_SPARKLE_1						,
    //	ALPHA_SPARKLE_2						,
    //	ALPHA_SPARKLE_3						,
    // 	ALPHA_FIREWORKS						,
    // 	ALPHA_WAVES							,
    ALPHA_THROB							,
    ALPHA_SOLID							,
    ALPHA_COUNT							, // Always equals the count of the previous list of menu items, so long as MENU_START = 0
    ALPHA_TITLE				= MENU_TITLE,
    ALPHA_SIZE				= MENU_SIZE
} AlphaMode; // not finished

typedef enum
{
    COLOR_RED				= MENU_START,
    COLOR_ORANGE						,
    COLOR_YELLOW						,
    COLOR_GREEN							,
    COLOR_TEAL							,
    COLOR_BLUE							,
    COLOR_SKY							,
    COLOR_VIOLET						,
    COLOR_PINK							,
    COLOR_WHITE							,
    COLOR_RAINBOW_SOLID					,
    COLOR_RAINBOW_LEFT					,
    COLOR_RAINBOW_RIGHT					,
    COLOR_RAINBOW_UP					,
    COLOR_RAINBOW_DOWN					,
    COLOR_RAINBOW_SPIN					,
// 	COLOR_RAINBOW_ROLL_L4				,
// 	COLOR_RAINBOW_ROLL_R1				,
// 	COLOR_RAINBOW_ROLL_R2				,
// 	COLOR_RAINBOW_ROLL_R3				,
// 	COLOR_RAINBOW_ROLL_R4				,
    COLOR_AS_A_PATTERN_START			,
    COLOR_COP_LIGHTS		= COLOR_AS_A_PATTERN_START,
// 	COLOR_HAZARD_2						,
// 	COLOR_HAZARD_3						,
    COLOR_FIRE							,
    COLOR_DISCO							,
    COLOR_WAVES							,
    COLOR_CUSTOM_SOLID					,
    COLOR_COUNT							, // Always equals the count of the previous list of menu items, so long as MENU_START = 0
    COLOR_TITLE				= MENU_TITLE,
    COLOR_MENU_SIZE			= MENU_SIZE
} ColorMode;

typedef enum
{
    COLOR_HSV_RED			= 0,
    COLOR_HSV_ORANGE		= 15,
    COLOR_HSV_YELLOW		= 50,
    COLOR_HSV_LIME_GREEN	= 72,
    COLOR_HSV_GREEN			= 86,
    COLOR_HSV_TEAL			= 105,
    COLOR_HSV_BLUE			= 172,
    COLOR_HSV_SKY			= 150,
    COLOR_HSV_VIOLET		= 190,
    COLOR_HSV_PINK			= 234,
    COLOR_HSV_END			= 255,
    COLOR_HSV_WHITE			,
    COLOR_HSV_CUSTOM		,
    COLOR_HSV_RAINBOW
} ColorHsvValue;

typedef enum
{
    EFFECT_GHOST			= MENU_START,//leaves fading tracers on display
    EFFECT_GLOW							,//if the array value is zero a background glow is inserted
    EFFECT_COLOR						,//color correction
    EFFECT_SHINE						,//intensity correction
    EFFECT_SPEED						,//400/800kHz switch
    EFFECT_MENU							,//on LED menu, flag only
    EFFECT_TITLE			= MENU_TITLE,
    EFFECT_MENU_SIZE		= MENU_SIZE
} EffectMode;

typedef enum {
    FORWARD,
    BACKWARD
} Direction;

typedef enum
{
    MOTION_BACKWARD_6		= MENU_START,
    MOTION_BACKWARD_5					,
    MOTION_BACKWARD_4					,
    MOTION_BACKWARD_3					,
    MOTION_BACKWARD_2					,
    MOTION_BACKWARD_1					,
    MOTION_STILL						,
    MOTION_FORWARD_1					,
    MOTION_FORWARD_2					,
    MOTION_FORWARD_3					,
    MOTION_FORWARD_4					,
    MOTION_FORWARD_5					,
    MOTION_FORWARD_6					,
    MOTION_COUNT						, // Always equals the count of the previous list of menu items, so long as MENU_START = 0
    MOTION_TITLE			= MENU_TITLE,
    MOTION_SIZE				= MENU_SIZE
} MotionMode;

typedef enum
{
    BRIGHT_1					= MENU_START,
    BRIGHT_DEFAULT				= 10		,
    BRIGHT_MINIMUM				= 5			,
    BRIGHT_MAXIMUM				= 100		,
    BRIGHT_INCREMENT			= 5			,
    BRIGHT_TITLE				= MENU_TITLE,
    BRIGHT_SIZE					= MENU_SIZE
} BrightnessMode;

enum wsSpeed {
    SLOW,
    FAST
};

enum runtimeFlagIndex {
    WS_PLUGGED_IN			,//= 0x01,
    MICROPHONE_ON			,//= 0x02,
    SOUND_SAMPLE_READY		,//= 0x04,
    ACCELEROMETER_ON		,//= 0x08,
    POWER_SUPPLY_COLLAPSE	,//= 0x10,
    //   = 0x20,
    //   = 0x40,
    //   = 0x80
};

enum lcdFlagsIndex {
    DISPLAY_EFFECT_FLAG_STATUS	= 0x01
    //	 = 0x02,
    //	 = 0x04,
    //	 = 0x08,
    //	 = 0x10,
    //   = 0x20,
    //   = 0x40,
    //   = 0x80
};

#endif /* MODES_H_ */
