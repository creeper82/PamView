#include "pixel.h"

Pixel::Pixel(u_int8_t red, u_int8_t green, u_int8_t blue)
{
    r = red;
    g = green;
    b = blue;
}

Pixel::Pixel() : Pixel(255, 255, 255) {}

u_int8_t Pixel::getGrayscaleValue()
{
    return (r + g + b) / 3;
}

std::ostream &operator<<(std::ostream &stream, const Pixel &pixel)
{
    stream << "(" << +pixel.r << ", " << +pixel.g << ", " << +pixel.b << ")";
    return stream;
}

Pixel PixelTransformations::grayscale(Pixel pixel)
{
    u_int8_t v = pixel.getGrayscaleValue();
    return Pixel(v, v, v);
}

Pixel PixelTransformations::negative(Pixel pixel)
{
    return Pixel(255 - pixel.r, 255 - pixel.g, 255 - pixel.b);
}

Pixel PixelTransformations::brightness(Pixel pixel, int level)
{
    return pixel;
}

Pixel PixelTransformations::contrast(Pixel pixel, int level)
{
    return pixel;
}

Pixel PixelTransformations::saturation(Pixel pixel, int level)
{
    return pixel;
}
