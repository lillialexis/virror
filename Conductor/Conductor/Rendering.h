//
// Created by Lilli Szafranski on 5/7/18.
//

#ifndef VIRROR_CONDUCTOR_RENDERING_H
#define VIRROR_CONDUCTOR_RENDERING_H

#import "Util.h"
#import "Globals.h"

void renderFrame(RGB leds[], u_long frameNumber);
void addScannerValues(RGB leds[], uint16_t scannerValues[]);

#endif //VIRROR_CONDUCTOR_RENDERING_H
