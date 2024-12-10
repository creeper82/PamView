#pragma once
#include <iostream>
#include <optional>
#include "pixel.h"

// Represents the status after reading the bitmap from a stream
enum BITMAP_LOAD_STATUS
{
    SUCCESS,
    BAD_DIMENSIONS,
    TOO_LARGE,
    UNSUPPORTED_FORMAT,
    UNSUPPORTED_MAXVALUE,
    STREAM_CORRUPT_EOF,
    STREAM_CORRUPT
};

// Represents the Portable AnyMap variant (P-number)
enum FILETYPE {
    // Portable BitMap, a dual color (black or white) bitmap represented using ASCII
    P1,
    // Portable GrayMap, a grayscale (0~255) bitmap represented using ASCII
    P2,
    // Portable PixMap, an RGB (0~255 x 3) bitmap represented using ASCII 
    P3
};

// Represents the bitmap and dimensions at some point in the past, to undo the changes into old state.
struct SavedBitmapState {
    Pixel** map = nullptr;
    int width = 0;
    int height = 0;

    SavedBitmapState(Pixel **_map, int _width, int _height)
        : map(_map), width(_width), height(_height) {};

    SavedBitmapState() : SavedBitmapState(nullptr, 0, 0) {};
};

// Represents a 2D bitmap, saves and loads the bitmap, handles image transformations.
class Bitmap {
    // handles a P3 bitmap (PPM)
    public:
        // Returns the bitmap width.
        int getWidth();

        // Returns the bitmap height.
        int getHeight();

        // Returns the number of bytes occupied by the loaded bitmap.
        size_t getBitmapMemUsage();
        
        // Returns the number of bytes occupied by the undo stack history, if exists.
        size_t getUndoStackMemUsage();

        // Returns the number of bytes occupied by both the bitmap and undo stack history.
        size_t getTotalMemUsage();

        // Returns if any bitmap is open (allocated).
        bool hasOpenBitmap();

        // Returns the pixel at given coordinantes.
        Pixel getPixelAt(int x, int y);

        bool setPixelAt(int x, int y, Pixel newPixel);

        // Updates the bitmap dimensions and clears the bitmap, with possibility to select a fill color. Overrides existing bitmap and clears the undo history.
        void createBlank(int width, int height, Pixel defaultFill = Pixel());

        // Clears the bitmap, with possibility to select a fill color.
        void fillBitmap(Pixel defaultFill = Pixel());

        // Closes the bitmap if open, and frees the memory. Use createBlank to create.
        void closeBitmap();

        // Reads PPM bitmap file from stream and overrides the current image.
        BITMAP_LOAD_STATUS openStream(std::istream &stream, void (*progressHandler)(int progressPercent) = nullptr);

        // Saves the PPM bitmap to a stream, based on given filetype (P-number).
        bool saveToStream(std::ostream &stream, FILETYPE filetype, void (*progressHandler)(int) = nullptr);

        // Transforms the image based on given transformation function.
        void transformImage(Pixel (*transformFunction)(Pixel));

        // Transforms the image based on given transformation function and strength/level of the transformation.
        void transformImage(Pixel (*transformFunctionWithLevel)(Pixel, int), int);

        // Undo the last change, and load previous bitmap state, if exists. Related: canUndo()
        void undoLastChange();

        // Returns if undo operation is available.
        bool canUndo();

        // Creates an empty white 100 x 100 bitmap.
        Bitmap();

        // Creates an empty bitmap of given dimensions, with possibility to set a default color.
        Bitmap(int initialWidth, int initialHeight, Pixel defaultFill = Pixel());
    private:
        void freeMemory();
        void allocateBitmapMemory(int width, int height);
        void commitPreChange();
        void clearUndoHistory();
        size_t getMapMemoryUsage(int width, int height);
        std::optional<SavedBitmapState> previousBitmapState { };
        int width = 0;
        int height = 0;
        bool hasPoint(int x, int y);
        Pixel** map = nullptr;
};