#pragma once
#include "pixel.h"
#include "cmath"
// #include <cstdint>

struct RGBColor {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;

    RGBColor();
    RGBColor(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {};
    RGBColor(Pixel pixel) : r(pixel.r), g(pixel.g), b(pixel.b) {};
};

struct HSVColor {
    // Hue (0-360)
    int hue = 0;
    // Saturation (0-1)
    float saturation = 0;
    // Brightness (0-100)
    int brightness = 100;

    HSVColor();
    HSVColor(int _hue, float _saturation, int _brightness) : hue(_hue), saturation(_saturation), brightness(_brightness) {};
};

namespace Conversions {
    RGBColor HSVToRGB(HSVColor hsv);
    HSVColor RGBToHSV(RGBColor rgb);
    HSVColor RGBToHSV(Pixel pixel);
}