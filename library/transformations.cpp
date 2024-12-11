#include "transformations.h"
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
        return pixel;
    }

    Pixel contrast(Pixel pixel, int level)
    {
        return pixel;
    }

    Pixel saturation(Pixel pixel, int level)
    {
        return pixel;
    }
}
