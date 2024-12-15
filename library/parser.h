#pragma once
#include "bitmap.h"
#include <cstdint>

class Parser
{
public:
    static BITMAP_LOAD_STATUS loadToBitmap(Bitmap &bitmap, std::istream &stream, void (*progressHandler)(int) = nullptr);
    static bool saveBitmapTo(Bitmap &bitmap, std::ostream &stream, FILETYPE filetype, void (*progressHandler)(int) = nullptr);

private:
    static std::string readStringSkipComment(std::istream &stream);
    static int readIntSkipComment(std::istream &stream);
    static char readRawChar(std::istream &stream);
    static Pixel readPixel(std::istream &stream, FILETYPE filetype);
    static void throwExceptions(std::istream &stream);
    static void consumeEmptyLines(std::istream &stream);
};