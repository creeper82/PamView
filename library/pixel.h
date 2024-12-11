#pragma once
#include <iostream>
#include <cstdint>

class Pixel {
    public:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        Pixel(uint8_t r, uint8_t g, uint8_t b);
        // Creates a default white pixel
        Pixel();

        // Gets the grayscale value of this pixel (R, G, and B are equal in grayscale).
        uint8_t getGrayscaleValue();

        friend std::ostream &operator<<(std::ostream &stream, const Pixel &pixel);
};

namespace PixelTransformations {
    // Returns the pixel with adjusted brightness (level -100 to 100).
    Pixel brightness(Pixel pixel, int level);

    // Returns the pixel with adjusted contrast (level -100 to 100).
    Pixel contrast(Pixel pixel, int level);

    // Returns the pixel with adjusted saturation (level -100 to 100).
    Pixel saturation(Pixel pixel, int level);

    // Returns the pixel but in grayscale.
    Pixel grayscale(Pixel pixel);

    // Returns the pixel but in negative.
    Pixel negative(Pixel pixel);
}