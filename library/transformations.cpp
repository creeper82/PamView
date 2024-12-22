#include "transformations.h"
#include "color.h"

namespace PixelTransformations
{
    Pixel grayscale(Pixel pixel)
    {
        uint8_t v = pixel.getGrayscaleValue();
        return Pixel(v, v, v);
    }

    Pixel negative(Pixel pixel)
    {
        return Pixel(255 - pixel.r, 255 - pixel.g, 255 - pixel.b);
    }

    Pixel brightness(Pixel pixel, int level)
    {
        HSVColor hsv = Conversions::RGBToHSV(pixel);
        hsv.brightness += level;
        if (hsv.brightness < 0)
            hsv.brightness = 0;
        if (hsv.brightness > 255)
            hsv.brightness = 255;
        RGBColor rgb = Conversions::HSVToRGB(hsv);
        Pixel brightnessChanged = Pixel(rgb.r, rgb.g, rgb.b);
        return brightnessChanged;
    }

    Pixel contrast(Pixel pixel, int level)
    {
        return pixel;
    }

        Pixel saturation(Pixel pixel, int level)
        {
            if (pixel.r == pixel.g && pixel.g == pixel.b) return pixel;

        HSVColor hsv = Conversions::RGBToHSV(pixel);

        hsv.saturation += ((float)level / 100);

        if (hsv.saturation < 0)
            hsv.saturation = 0;

        if (hsv.saturation > 1)
            hsv.saturation = 1;

        RGBColor rgb = Conversions::HSVToRGB(hsv);
        Pixel saturated = Pixel(rgb.r, rgb.g, rgb.b);
        return saturated;
    }

    Pixel blacknwhite(Pixel pixel)
    {
        int luminance = 0.3*pixel.r + 0.587*pixel.g + 0.114*pixel.b;
        if (luminance > 127) return Pixel(255, 255, 255);
        else return Pixel(0, 0, 0);
    }
}

namespace PixelCombinations {
    Pixel add(Pixel p1, Pixel p2) {
        Pixel result;
        result.r = ((float)p1.r / 2 + (float)p2.r / 2);
        result.g = (float)p1.g / 2 + (float)p2.g / 2;
        result.b = (float)p1.b / 2 + (float)p2.b / 2;
        return result;
    }

    Pixel substract(Pixel p1, Pixel p2) {
        Pixel result;
        result.r = std::max((int)p1.r - (int)p2.r, 0);
        result.g = std::max((int)p1.g - (int)p2.g, 0);
        result.b = std::max((int)p1.b - (int)p2.b, 0);
        return result;
    }

    Pixel multiply(Pixel p1, Pixel p2) {
        Pixel result;
        result.r = ((p1.r / 255.0) * (p2.r / 255.0)) * 255;
        result.g = ((p1.g / 255.0) * (p2.g / 255.0)) * 255;
        result.b = ((p1.b / 255.0) * (p2.b / 255.0)) * 255;
        return result;
    }
}