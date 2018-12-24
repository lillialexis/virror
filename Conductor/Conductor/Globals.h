//
// Created by Lilli Szafranski on 5/7/18.
//

#ifndef VIRROR_CONDUCTOR_GLOBALS_H
#define VIRROR_CONDUCTOR_GLOBALS_H

#include <sys/types.h>
#include "Defines.h"
#include "Util.h"
#include "Modes.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * Frame-rate/Loop stuff * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern u_long frameNumber; // TODO: Make this better controlled (i.e., only main can update it) global var
extern int    totalLoopDuration;
extern int    timeToExit;

//extern int menuClock;
//extern int screenClock;
//extern int bluetoothClock;

//RGB ledArray [LED_DISPLAY_SIZE];

Clock getAlphaClock();
Clock getColorClock();

void updateClocks();
void updateWaves();

#define WAVE_SET_SIZE 1//PATTERN_COUNT
extern Wave waveSets [WAVE_SET_SIZE];

extern HSV customColor;

extern uint8_t effectFlags;     //bit flags defines what effects are used

ColorMode getColorMode();
void setColorMode(ColorMode mode);

AlphaMode getAlphaMode();
void setAlphaMode(AlphaMode mode);

void setMotionMode(MotionMode mode);
MotionMode getMotionMode();

//void loadBrightness();
//void saveBrightness();

//uint8_t getBrightness();
//void setBrightness(uint8_t mode);

uint8_t getGlobalIntensity();

uint8_t getEffectFlags();
void setEffectFlags(uint8_t flag);

//uint8_t getLoadPos();
//void setLoadPos(uint8_t mode);
//
//uint8_t getSavePos();
//void setSavePos(uint8_t mode);

#define SOUND_BUFFER_SIZE 50
extern uint8_t soundBuffer[SOUND_BUFFER_SIZE];
extern uint8_t soundPointer;
extern Sound soundResults;
extern Sound soundGain;
extern Sound soundDecayLoops;
#define SOUND_DECAY_LOOP_RESET FRAME_RATE * 5
#define SOUND_DECAY_RATE -1

//extern uint8_t EEMEM colorPreset[10];
//extern uint8_t EEMEM patternPreset[10];
//extern uint8_t EEMEM motionPreset[10];
//extern uint8_t EEMEM effectPreset[10];
//extern uint8_t EEMEM brightSave;
//extern uint8_t EEMEM lastPreset;
//extern uint8_t EEMEM safePreset;

#define PRINT_BUFFER_SIZE 16
extern const char printBufferLine1[PRINT_BUFFER_SIZE + 1];
extern const char printBufferLine2[PRINT_BUFFER_SIZE + 1];
void clearPrintBuffer(char *printBuffer);




#endif //VIRROR_CONDUCTOR_GLOBALS_H
