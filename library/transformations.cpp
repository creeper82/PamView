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
}
