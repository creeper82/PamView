#pragma once
#include <iostream>
#include "pixel.h"

// Represents the status after reading the bitmap from a stream
enum BITMAP_LOAD_STATUS
{
    SUCCESS,
    BAD_DIMENSIONS,
    TOO_LARGE,
    UNSUPPORTED_FORMAT,
    UNSUPPORTED_MAXVALUE
};

// Represents a 2D bitmap, saves and loads the bitmap, handles image transformations.
class Bitmap {
    // handles a P3 bitmap (PPM)
    public:
        // Returns the bitmap width.
        int getWidth();

        // Returns the bitmap height.
        int getHeight();

        // Returns the pixel at given coordinantes.
        Pixel getPixelAt(int x, int y);

        bool setPixelAt(int x, int y, Pixel newPixel);

        // Updates the bitmap dimensions and clears the bitmap, with possibility to select a fill color.
        void setDimensionsAndClear(int width, int height, Pixel defaultFill = Pixel());

        // Clears the bitmap, with possibility to select a fill color.
        void clear(Pixel defaultFill = Pixel());

        // Reads PPM bitmap file from stream and overrides the current image.
        BITMAP_LOAD_STATUS openStream(std::istream& stream);

        // Transforms the image based on given transformation function.
        void transformImage(Pixel (*transformFunction)(Pixel));

        // Transforms the image based on given transformation function and strength/level of the transformation.
        void transformImage(Pixel (*transformFunctionWithLevel)(Pixel, int), int);

        // Creates an empty white 100 x 100 bitmap.
        Bitmap();

        // Creates an empty bitmap of given dimensions, with possibility to set a default color.
        Bitmap(int initialWidth, int initialHeight, Pixel defaultFill = Pixel());
    private:
        void freeMemory();
        int width;
        int height;
        bool hasPoint(int x, int y);
        Pixel** map;
};