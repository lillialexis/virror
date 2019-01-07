//
// Created by Lilli Szafranski on 2019-01-04.
//

#ifndef RAINBOWDISPLAYTEST_DEFINES_H
#define RAINBOWDISPLAYTEST_DEFINES_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * DEVELOPER TOGGLES * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define DEV_RIG
//#define REAL_RIG
//#define REAL_RIG_HALF

#define DEV_SCAN
//#define REAL_SCAN

#define RESET_MODE_CHANGE_TRIGGERS_ON_POSITIVE // TODO: Test which is better with real sensor data

#define USING_BKG_TEST_MODES
#define USING_MODE_CHANGE_DEV_BOUNDARIES
//#define USING_MOVING_SCAN_CIRCLE
#define USING_EMPTY_SCAN_DATA


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * HARDWARE CONSTANTS  * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef DEV_RIG
#define LED_WIDTH  16
#define LED_HEIGHT 16
#endif

#ifdef REAL_RIG
#define LED_WIDTH  64
#define LED_HEIGHT 48
#endif

#ifdef REAL_RIG_HALF
#define LED_WIDTH  32
#define LED_HEIGHT 48
#endif

#define SCAN_WIDTH  16//80
#define SCAN_HEIGHT 16//14

#define BYTE_MAX 255

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * MODE CHANGE TYPES AND CONSTANTS * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* If a mode change isn't triggered by the user, this is how long a mode runs before
 * changing automatically. */
//#define MODE_CHANGE_DEFAULT_TIMEOUT 100000
//#define MODE_CHANGE_DEFAULT_TIMEOUT 20000
#define MODE_CHANGE_DEFAULT_TIMEOUT 2000

/* This is how long it takes the mode to fade in/out */
#define MODE_CHANGE_FADE_DURATION 127

/* These are the states of the foreground and background modes. */
enum ModeChangeMask {
    NO_MODE_CHANGE = 0,
    FOREGROUND_MODE_CHANGE_FLAG = 1,
    BACKGROUND_MODE_CHANGE_FLAG = 2
};

typedef struct {
    /* Values to be used by a mode, pseudo-randomly set by processing a row of scan data
     * on the frame that a new background or foreground mode begins. Can be used to adjust
     * speed, color, etc. of the new mode or ignored. */
    byte modeVariant1; /* This variant can have a value of 0 or 1     */
    byte modeVariant2; /* This variant can have a value of 0, 1, or 2 */
    byte modeVariant3; /* This variant can have a value of 0-3        */
    byte modeVariant4; /* This variant can have a value of 0-4        */
    byte modeVariant5; /* This variant can have a value of 0-255      */
} ModeVariants;

/* Lets put a couple of "detection windows" in the top-left and top-right areas of our scans.
 * If we detect something (e.g., a hand) in our detection window, trigger a mode change. The
 * scan data in our detection windows are added up and if the cumulative value passes the
 * MODE_CHANGE_DETECTION_THRESHOLD, a mode change is triggered. We can tweak these constants
 * to values that make sense and are the right amount of "sensitive" once we have the real scan
 * hardware set up. */
#ifdef USING_MODE_CHANGE_DEV_BOUNDARIES
#define MODE_CHANGE_DETECTION_THRESHOLD 100
#define MODE_CHANGE_DETECTION_WIDTH     4//15 // TODO: Change once scaling is implemented
#define MODE_CHANGE_DETECTION_HEIGHT    4//3 // TODO: Change once scaling is implemented
#define POS_COUNTER_TIMEOUT   300
#define NEG_COUNTER_TIMEOUT   100
#else
#define MODE_CHANGE_DETECTION_THRESHOLD 40
#define MODE_CHANGE_DETECTION_WIDTH     15
#define MODE_CHANGE_DETECTION_HEIGHT    3
#define POS_COUNTER_TIMEOUT   200
#define NEG_COUNTER_TIMEOUT   100
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * COLOR TYPES AND CONSTANTS * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct {
    byte h;
    byte s;
    byte l;
} HSL;

typedef struct {
    byte r;
    byte g;
    byte b;
} RGB;

typedef struct {
    byte h;
    byte s;
    byte v;
} HSV;

#define DEFAULT_SATURATION 255
#define DEFAULT_BRIGHTNESS 255

#define MAX_ALPHA      BYTE_MAX
#define MAX_SATURATION BYTE_MAX
#define MAX_BRIGHTNESS BYTE_MAX
#define MAX_SCAN_VALUE BYTE_MAX

#define H_RED     0
#define H_GREEN   85
#define H_BLUE    170
#define H_YELLOW  42
#define H_CYAN    127
#define H_MAGENTA 212

const HSV HSV_RED     = { H_RED, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };
const HSV HSV_GREEN   = { H_GREEN, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };
const HSV HSV_BLUE    = { H_BLUE, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };
const HSV HSV_YELLOW  = { H_YELLOW, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };
const HSV HSV_CYAN    = { H_CYAN, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };
const HSV HSV_MAGENTA = { H_MAGENTA, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS };

#define MAX_SIMPLE_COLORS 6
HSV SIMPLE_COLORS[] = {HSV_RED, HSV_GREEN, HSV_BLUE, HSV_YELLOW, HSV_CYAN, HSV_MAGENTA };


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * MOTION TYPES AND CONSTANTS  * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef enum {
    LEFT,
    RIGHT
} xDirection;

typedef enum {
    UP,
    DOWN
} yDirection;

typedef struct {
    float x;
    float y;
    float width;
    float height;
    float rise;
    float run;
    xDirection xDir;
    yDirection yDir;
    float radius;
} Circle;

typedef struct {
    bool active;
    float x;
    float y;
    float radius;
    unsigned int waveDuration;
    unsigned int totalDuration;
    unsigned int currentFrame;
    unsigned int waves;
} Ripple;

#define ACTIVE_RIPPLE                 true
#define DEFAULT_RIPPLE_RADIUS         8
#define DEFAULT_RIPPLE_WAVE_DURATION  10
#define DEFAULT_RIPPLE_TOTAL_DURATION (MODE_CHANGE_FADE_DURATION * 2)
#define DEFAULT_RIPPLE_CURRENT_FRAME  0
#define DEFAULT_RIPPLE_NUM_WAVES      4

#define TODO_FIXME_RIPPLE_X_1 14
#define TODO_FIXME_RIPPLE_X_2 2
#define TODO_FIXME_RIPPLE_Y   2

#define MAX_RIPPLES 10

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * MACROS  * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ppm(a, b) ((((a) + 1) % (b)))


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * OTHER TYPES AND CONSTANTS * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif //RAINBOWDISPLAYTEST_DEFINES_H
