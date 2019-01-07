#include "Defines.h"

float feedbackArray[LED_WIDTH * LED_HEIGHT];
Ripple rippleArray[MAX_RIPPLES];

int rippleCount = 0;

void feedbackSetup() {
    memset(feedbackArray, 0, LED_WIDTH * LED_HEIGHT * sizeof(float));
    memset(rippleArray, 0, MAX_RIPPLES * sizeof(Ripple));
}

void addRipple(Ripple newRipple) {
    Serial.println("ar");

    /* Find the oldest and replace it... */
    unsigned int smallestRemaining = 100000; // whatever, really big number
    unsigned int smallestIndex = MAX_RIPPLES;

    for (int i = 0; i < MAX_RIPPLES; i++) {
        unsigned int currentRemaining =
                rippleArray[i].totalDuration - rippleArray[i].currentFrame;

        if (currentRemaining == 0) { /* Then this ripple is dead; use its slot. */
            Serial.println("cr0");

            rippleArray[i] = newRipple;
            return;
        }

        if (currentRemaining < smallestIndex) {
            Serial.println("cr < ci");

            smallestRemaining = currentRemaining;
            smallestIndex = i;
        }
    }

    if (smallestIndex == MAX_RIPPLES) { /* There was an error I guess... */
        Serial.println("error");

        return;
    }

    Serial.print("ra index: ");
    Serial.println(smallestIndex);

    rippleArray[smallestIndex] = newRipple;
}

void applyFeedback(HSV ledArray[], unsigned int width, unsigned int height) {
    memset(feedbackArray, 0, LED_WIDTH * LED_HEIGHT * sizeof(int));

    bool haveAnyActiveRipples = false;

    for (int i = 0; i < MAX_RIPPLES; i++) {
        Ripple ripple = rippleArray[i];

        if (ripple.active) {
            haveAnyActiveRipples = true;

            Serial.println("ar");

            applyRipple(ripple, width, height);

            ripple.currentFrame++;
            if (ripple.totalDuration - ripple.currentFrame == 0) {
                Serial.println("ro");

                memset(&ripple, 0, sizeof(ripple));
            }
        }

        rippleArray[i] = ripple;
    }

    if (haveAnyActiveRipples) {
        applyFeedbackAlpha(ledArray, width, height);
    }
}

void applyRipple(Ripple ripple, unsigned int width, unsigned int height) {
    int xMin = max(0, ripple.x - ripple.radius), xMax = min(ripple.x + ripple.radius, width);
    int yMin = max(0, ripple.y - ripple.radius), yMax = min(ripple.y + ripple.radius, height);

    for (int x = xMin; x < xMax; x++) {
        for (int y = yMin; y < yMax; y++) {
            float percentThrough = (float)ripple.currentFrame / (float)ripple.totalDuration;
            float currentOuterRadius = (float)ripple.radius * percentThrough;
            float currentInnerRadius = currentOuterRadius - ((float)ripple.radius / (float)ripple.waves);

            float distance = dist(ripple.x, ripple.y, x, y);

            if (distance < currentOuterRadius && distance > currentInnerRadius) {
                feedbackArray[rc2iLeds(y, x)] =
                        min(1.0, feedbackArray[rc2iLeds(y, x)] +
                                    ((ripple.radius - distance) / ripple.radius) * (1.0 - percentThrough) * 0.50);
            }

            // TODO: Add multiple waves and stuff
        }
    }
}

void applyFeedbackAlpha(HSV ledArray[], unsigned int width, unsigned int height) {
    for (int i = 0; i < width * height; i++) {
        ledArray[i] =
                { ledArray[i].h, round(((float)(ledArray[i].s)) * (1.0 - feedbackArray[i])), ledArray[i].v };

    }
}
