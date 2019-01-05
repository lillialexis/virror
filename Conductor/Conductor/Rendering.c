//
// Created by Lilli Szafranski on 5/7/18.
//

#include "Rendering.h"
#include "Includes.h"
#include "AlphaValue.h"
#include "ColorValue.h"

void motionUpdate()
{
    updateClocks();
    updateWaves();

//	for (int x = 0; x < 8; x++)	{waveUpdate(x);}
//	//update clocks
//	Clock clock = alphaClock();
//	clockUpdate();
//	clockUpdate(&colorClock());
    //move coordinates

//    if(motionMode == MOTION_BACKWARD_2) {
//        char temp[20];
//        sprintf(temp, "clock: %u", alphaClock.time);
//        lcdReplaceLine(0, temp);//itoa(alphaClock.time, temp, 10));
//        _delay_ms(500);
//    }
}

void preRenderEffects()
{
    if (effectFlags & (1<<EFFECT_GHOST))
    {
        //make the current array values dim a bit
        for(int x = 0; x < LED_DISPLAY_SIZE; x++)
        {
            ledArray[x].g = (ledArray[x].g >> 1); //divide value by 2
            ledArray[x].r = (ledArray[x].r >> 1);
            ledArray[x].b = (ledArray[x].b >> 1);
        }
    }
    //pattern should store zero when no data, no need to clear
    /*if (effectFlags &= (1<<EFFECT_GHOST) == TRUE)
    {
        //clear the array
        fillWsData(0,0,0);
    }*/
}

RGB renderEffects(Coordinate loc, uint16_t ledNumber, RGB input) {
    if (effectFlags & (1<<EFFECT_GHOST))
    {
        //store not zero
    }
    if (effectFlags & (1<<EFFECT_GLOW))
    {
        //store glow if zero
    }
    if (effectFlags & (1<<EFFECT_SHINE))
    {
        //store glow if zero
    }
    return input;
}

void postRenderEffects() {
    if (effectFlags & (1<<EFFECT_MENU))
    {
        //on LED menu is active
    }
}

uint8_t effectSpeedFlag() {
    return (effectFlags & (1<<EFFECT_SPEED));
}

void renderFrame(RGB leds[], u_long frameNumber) {
    preRenderEffects();
    motionUpdate();
    //soundScan();
    Coordinate tempCord;
    uint8_t intensity;
    RGB tempRGB;

    for(uint8_t i = 0; i < LED_DISPLAY_SIZE; i++) { // build array loop
        tempCord.x = xPosition(i);
        tempCord.y = yPosition(i);
        intensity = renderAlphaValue(tempCord, i);
        tempRGB = renderColorValue(intensity, tempCord, i);//, tempRGB);
        tempRGB = renderEffects(tempCord, i, tempRGB);
        leds[i] = tempRGB;
        //soundSample();
    }

    postRenderEffects();
}

void addScannerValues(RGB leds[], uint16_t scannerValues[]) {
    // TODO
}
