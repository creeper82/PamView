#include "color.h"

RGBColor Conversions::HSVToRGB(HSVColor hsv)
{
    RGBColor result;

    int hue = hsv.hue;
    float saturation = hsv.saturation;
    int brightness = hsv.brightness;

    float chroma = brightness * saturation;
    float fHPrime = fmod(hue / 60.0, 6);
    float fX = chroma * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = brightness - chroma;

    if (0 <= fHPrime && fHPrime < 1)
    {
        result.r = chroma;
        result.g = fX;
        result.b = 0;
    }
    else if (1 <= fHPrime && fHPrime < 2)
    {
        result.r = fX;
        result.g = chroma;
        result.b = 0;
    }
    else if (2 <= fHPrime && fHPrime < 3)
    {
        result.r = 0;
        result.g = chroma;
        result.b = fX;
    }
    else if (3 <= fHPrime && fHPrime < 4)
    {
        result.r = 0;
        result.g = fX;
        result.b = chroma;
    }
    else if (4 <= fHPrime && fHPrime < 5)
    {
        result.r = fX;
        result.g = 0;
        result.b = chroma;
    }
    else if (5 <= fHPrime && fHPrime < 6)
    {
        result.r = chroma;
        result.g = 0;
        result.b = fX;
    }
    else
    {
        result.r = 0;
        result.g = 0;
        result.b = 0;
    }

    result.r += fM;
    result.g += fM;
    result.b += fM;

    return result;
}

HSVColor Conversions::RGBToHSV(RGBColor rgb)
{
    uint8_t r = rgb.r;
    uint8_t g = rgb.g;
    uint8_t b = rgb.b;
    HSVColor result;

    uint8_t colorMax = std::max(r, std::max(g, b));
    uint8_t colorMin = std::min(rgb.r, std::min(g, b));
    uint8_t delta = colorMax - colorMin;

    // Hue
    if (delta == 0)
    {
        result.hue = 0;
    }
    else
    {
        if (colorMax == r)
        {
            result.hue = static_cast<int>(60 * ((g - b) / static_cast<float>(delta)));
        }
        else if (colorMax == g)
        {
            result.hue = static_cast<int>(60 * ((b - r) / static_cast<float>(delta)) + 120);
        }
        else if (colorMax == b)
        {
            result.hue = static_cast<int>(60 * ((r - g) / static_cast<float>(delta)) + 240);
        }
        if (result.hue < 0)
        {
            result.hue += 360; // clamp hue value
        }
    }

    // Saturation
    if (colorMax == 0)
    {
        result.saturation = 0;
    }
    else
    {
        result.saturation = static_cast<float>(delta) / colorMax;
    }

    // Brightness (aka Value)
    result.brightness = colorMax;

    return result;
}

HSVColor Conversions::RGBToHSV(Pixel pixel)
{
    return RGBToHSV(RGBColor(pixel.r, pixel.g, pixel.b));
}

RGBColor::RGBColor()
{
}

HSVColor::HSVColor()
{
}