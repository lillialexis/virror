
// TODO: Where to do the color correction? Right now array of bytes. Should be to ints?
byte ColorCorrectLookup_old[256] = {0,	1,	1,	1,	2,	2,	2,	3,	3,	4,	4,	4,	5,	5,	5,	6,	6,	7,	7,	7,	8,	8,	8,	9,	9,	10,	10,	10,	11,	11,	12,	12,	13,	13,	13,	14,	14,	15,	15,	16,	16,	17,	17,	17,	18,	18,	19,	19,	20,	20,	21,	21,	22,	22,	23,	23,	24,	24,	25,	25,	26,	26,	27,	27,	28,	28,	29,	29,	30,	30,	31,	31,	32,	32,	33,	34,	34,	35,	35,	36,	37,	37,	38,	39,	39,	40,	41,	41,	42,	42,	43,	44,	44,	45,	46,	46,	47,	48,	48,	49,	50,	50,	51,	52,	53,	54,	55,	56,	57,	57,	58,	59,	60,	61,	62,	63,	64,	64,	65,	66,	67,	68,	69,	70,	71,	72,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	105,	106,	107,	108,	109,	110,	111,	112,	114,	115,	116,	117,	118,	119,	120,	121,	123,	124,	125,	126,	128,	129,	130,	131,	133,	134,	135,	137,	138,	139,	140,	142,	143,	144,	146,	147,	148,	149,	151,	152,	153,	155,	156,	158,	159,	161,	163,	164,	166,	167,	169,	171,	172,	174,	175,	177,	179,	180,	182,	183,	185,	187,	188,	190,	191,	193,	195,	197,	199,	201,	203,	205,	207,	209,	211,	213,	215,	217,	219,	221,	223,	225,	227,	229,	231,	233,	235,	237,	239,	241,	243,	245,	247,	249,	251,	253,	255};

int makeColor(HSV hsv) {
    return hsvToRgbInt(hsv);// makeColor(hsl.h, hsl.s, hsl.l);
}

// Convert HSL (Hue, Saturation, Lightness) to RGB (Red, Green, Blue)
//
//   hue:        0 to 359 - position on the color wheel, 0=red, 60=orange,
//                            120=yellow, 180=green, 240=blue, 300=violet
//
//   saturation: 0 to 100 - how bright or dull the color, 100=full, 0=gray
//
//   lightness:  0 to 100 - how light the color is, 100=white, 50=color, 0=black
//
int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
    unsigned int red, green, blue;
    unsigned int var1, var2;

    if (hue > 359) hue = hue % 360;
    if (saturation > 100) saturation = 100;
    if (lightness > 100) lightness = 100;

#ifdef DEV_RIG
    lightness = lightness / 4;
#endif

    // algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
    if (saturation == 0) {

        red = green = blue = lightness * 255 / 100;
    } else {

        if (lightness < 50) {
            var2 = lightness * (100 + saturation);
        } else {
            var2 = ((lightness + saturation) * 100) - (saturation * lightness);
        }

        var1 = lightness * 200 - var2;

        // TODO: Adding the color correction here doesn't look right. My guess is rounding/truncating issues? Confirm and fix by using RED_GRADIENT_MODE background mode
        red   = /*ColorCorrectLookup[min(*/h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000/*, BYTE_MAX)]*/;
        green = /*ColorCorrectLookup[min(*/h2rgb(var1, var2, hue) * 255 / 600000/*, BYTE_MAX)]*/;
        blue  = /*ColorCorrectLookup[min(*/h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000/*, BYTE_MAX)]*/;
    }

    return (red << 16) | (green << 8) | blue;
}

unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
    if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
    if (hue < 180) return v2 * 60;
    if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);
    return v1 * 60;
}

// alternate code:
// http://forum.pjrc.com/threads/16469-looking-for-ideas-on-generating-RGB-colors-from-accelerometer-gyroscope?p=37170&viewfull=1#post37170

int hsvToRgbInt_old(HSV hsv) {
    RGB rgb = hsvToRgb(hsv);

#ifdef DEV_RIG
    rgb.r = rgb.r / 4;
    rgb.g = rgb.g / 4;
    rgb.b = rgb.b / 4;
#endif

//    rgb.r = ColorCorrectLookup[rgb.r];
//    rgb.g = ColorCorrectLookup[rgb.g];
//    rgb.b = ColorCorrectLookup[rgb.b];

    return (rgb.r << 16) | (rgb.g << 8) | rgb.b;
}

RGB hsvToRgb_old(HSV hsv) {
    RGB rgb;
    unsigned char region, p, q, t;
    unsigned int h, s, v, remainder;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;

        return rgb;
    }

    // TODO: Where to color correct?

    // converting to 16 bit to prevent overflow
    h = /*ColorCorrectLookup[*/hsv.h/*]*/;
    s = /*ColorCorrectLookup[*/hsv.s/*]*/;
    v = /*ColorCorrectLookup[*/hsv.v/*]*/;

    region = h / 43;
    remainder = (h - (region * 43)) * 6;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = v;
            rgb.g = t;
            rgb.b = p;
            break;

        case 1:
            rgb.r = q;
            rgb.g = v;
            rgb.b = p;
            break;

        case 2:
            rgb.r = p;
            rgb.g = v;
            rgb.b = t;
            break;

        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = v;
            break;

        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = v;
            break;

        default:
            rgb.r = v;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}

HSV rgbToHsv_old(RGB rgb) {
    HSV hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * (long)(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}
