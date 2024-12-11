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