#pragma once
#include <functional>
#include <iostream>
#include <optional>
#include "pixel.h"

typedef std::function<void(int)> progressHandlerType;

typedef std::function<Pixel(Pixel)> transformType;
typedef std::function<Pixel(Pixel, int)> transformWithLevelType;

// Represents the Portable AnyMap variant (P-number)
enum FILETYPE
{
    // Portable BitMap, a dual color (black or white) bitmap represented using ASCII
    P1,
    // Portable GrayMap, a grayscale (0~255) bitmap represented using ASCII
    P2,
    // Portable PixMap, an RGB (0~255 x 3) bitmap represented using ASCII
    P3,
    // Portable BitMap, a dual color (black or white) bitmap represented in binary (raw)
    P4,
    // Portable GrayMap, a grayscale (0~255) bitmap represented in binary (raw)
    P5,
    // Portable PixMap, an RGB (0~255 x 3) bitmap represented in binary (raw)
    P6
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

        // Returns the pixel at given coordinantes, used for internal purposes. Skips a lot of checks.
        Pixel getPixelAtFast(int x, int y);

        bool setPixelAt(int x, int y, Pixel newPixel, bool skipCommit = false);

        // Quick pixel set, used for internal purposes. Skips a lot of checks.
        void setPixelAtFast(int x, int y, Pixel newPixel);

        // Updates the bitmap dimensions and clears the bitmap, with possibility to select a fill color. Overrides existing bitmap and clears the undo history.
        void createBlank(int width, int height, Pixel defaultFill = Pixel());

        // Clears the bitmap, with possibility to select a fill color.
        void fillWithColor(Pixel defaultFill = Pixel(), bool skipCommit = false);

        // Closes the bitmap if open, and frees the memory. Use createBlank to create.
        void closeBitmap();

        // Reads the bitmap file from stream and overrides the current image.
        void openFromStream(std::istream &stream, progressHandlerType progressHandler = nullptr);

        // Saves the PPM bitmap to a stream, based on given filetype (P-number).
        void saveToStream(std::ostream &stream, FILETYPE filetype = P3, progressHandlerType progressHandler = nullptr);

        // Transforms the image based on given transformation function.
        void transformImage(transformType, progressHandlerType progressHandler = nullptr);

        // Transforms the image based on given transformation function and strength/level of the transformation.
        void transformImage(transformWithLevelType, int, progressHandlerType progressHandler = nullptr);

        // Undo the last change, and load previous bitmap state, if exists. Related: canUndo()
        void undoLastChange();

        // Returns if undo operation is available.
        bool canUndo();

        // Creates an empty bitmap.
        Bitmap();

        // Creates an empty bitmap of given dimensions, with possibility to set a default color.
        Bitmap(int initialWidth, int initialHeight, Pixel defaultFill = Pixel());

        Bitmap(const Bitmap&) = delete;
        Bitmap& operator=(const Bitmap&) = delete;

        ~Bitmap();
    private:
        void freeMemory();
        void freePreviousBitmapStateMemory();
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