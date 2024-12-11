#include "pixel.h"

Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue)
{
    r = red;
    g = green;
    b = blue;
}

Pixel::Pixel() : Pixel(255, 255, 255) {}

uint8_t Pixel::getGrayscaleValue()
{
    return (r + g + b) / 3;
}

std::ostream &operator<<(std::ostream &stream, const Pixel &pixel)
{
    stream << "(" << +pixel.r << ", " << +pixel.g << ", " << +pixel.b << ")";
    return stream;
}