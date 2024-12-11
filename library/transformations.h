#pragma once
#include "pixel.h"

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