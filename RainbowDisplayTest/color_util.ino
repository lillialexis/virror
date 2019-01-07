
// TODO: Where to do the color correction? Right now array of bytes. Should be to ints?
byte ColorCorrectLookup[256] = {0,	1,	1,	1,	2,	2,	2,	3,	3,	4,	4,	4,	5,	5,	5,	6,	6,	7,	7,	7,	8,	8,	8,	9,	9,	10,	10,	10,	11,	11,	12,	12,	13,	13,	13,	14,	14,	15,	15,	16,	16,	17,	17,	17,	18,	18,	19,	19,	20,	20,	21,	21,	22,	22,	23,	23,	24,	24,	25,	25,	26,	26,	27,	27,	28,	28,	29,	29,	30,	30,	31,	31,	32,	32,	33,	34,	34,	35,	35,	36,	37,	37,	38,	39,	39,	40,	41,	41,	42,	42,	43,	44,	44,	45,	46,	46,	47,	48,	48,	49,	50,	50,	51,	52,	53,	54,	55,	56,	57,	57,	58,	59,	60,	61,	62,	63,	64,	64,	65,	66,	67,	68,	69,	70,	71,	72,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	105,	106,	107,	108,	109,	110,	111,	112,	114,	115,	116,	117,	118,	119,	120,	121,	123,	124,	125,	126,	128,	129,	130,	131,	133,	134,	135,	137,	138,	139,	140,	142,	143,	144,	146,	147,	148,	149,	151,	152,	153,	155,	156,	158,	159,	161,	163,	164,	166,	167,	169,	171,	172,	174,	175,	177,	179,	180,	182,	183,	185,	187,	188,	190,	191,	193,	195,	197,	199,	201,	203,	205,	207,	209,	211,	213,	215,	217,	219,	221,	223,	225,	227,	229,	231,	233,	235,	237,	239,	241,	243,	245,	247,	249,	251,	253,	255};

byte cc(byte c) {
    return ColorCorrectLookup[c];
}

int makeColor(HSV hsv) {
    return rgbToInt(hsvToRgb(hsv));
}

int rgbToInt(RGB rgb) {
#ifdef DEV_RIG
    rgb.r = rgb.r / 4;
    rgb.g = rgb.g / 4;
    rgb.b = rgb.b / 4;
#endif

    rgb.r = ColorCorrectLookup[rgb.r];
    rgb.g = ColorCorrectLookup[rgb.g];
    rgb.b = ColorCorrectLookup[rgb.b];

    return (rgb.r << 16) | (rgb.g << 8) | rgb.b;
}

RGB hsvToRgb(HSV hsv) {
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

    // converting to 16 bit to prevent overflow
    h = hsv.h;
    s = hsv.s;
    v = hsv.v;

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

HSV rgbToHsv(RGB rgb) {
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

HSV mixHsv(HSV background, HSV foreground, float foregroundOpacity) {
   static int counter = 0;

    if (counter == 5000) {
        printHsv("hsv1", background);
        printHsv("hsv2", foreground);
    }

    RGB backgroundRGB = hsvToRgb(background);
    RGB foregroundRGB = hsvToRgb(foreground);

    if (counter == 5000) {
        printRgb("rgb1", backgroundRGB);
        printRgb("rgb2", foregroundRGB);
    }

    byte newR = (byte)(((foregroundOpacity * (float)foregroundRGB.r) + ((1 - foregroundOpacity) * (float)backgroundRGB.r)));
    byte newG = (byte)(((foregroundOpacity * (float)foregroundRGB.g) + ((1 - foregroundOpacity) * (float)backgroundRGB.g)));
    byte newB = (byte)(((foregroundOpacity * (float)foregroundRGB.b) + ((1 - foregroundOpacity) * (float)backgroundRGB.b)));

    RGB rgb = { newR, newG, newB };
    HSV hsv = rgbToHsv(rgb);

    if (counter == 5000) {
        printRgb("new rgb", rgb);
        printHsv("new hsv", hsv);
    }

    counter++;

    return hsv;
}

HSV transitionHsv(HSV start, HSV stop, float percent) {

    RGB startRGB = hsvToRgb(start);
    RGB stopRGB  = hsvToRgb(stop);

    byte newR = (byte)(startRGB.r + (((float)(stopRGB.r - startRGB.r)) * percent));
    byte newG = (byte)(startRGB.g + (((float)(stopRGB.g - startRGB.g)) * percent));
    byte newB = (byte)(startRGB.b + (((float)(stopRGB.b - startRGB.b)) * percent));

    RGB rgb = { newR, newG, newB };
    HSV hsv = rgbToHsv(rgb);

    return hsv;
}

void printColor(String label, byte a, byte b, byte c) {
    String str = label;
    Serial.println(str + ": { " + a + ", " + b + ", " + c + " }");
}

void printHsv(String label, HSV hsv) {
    printColor(label, hsv.h, hsv.s, hsv.v);
}

void printRgb(String label, RGB rgb) {
    printColor(label, rgb.r, rgb.g, rgb.b);
}
