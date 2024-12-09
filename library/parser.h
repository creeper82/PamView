#pragma once
#include "bitmap.h"

class Parser
{
public:
    static BITMAP_LOAD_STATUS loadToBitmap(Bitmap& bitmap, std::istream& stream, void(*progressHandler)(int) = nullptr);

private:
    static std::string readStringSkipComment(std::istream &stream);
    static int readIntSkipComment(std::istream &stream);
    static Pixel readPixel(std::istream &stream);
    static void throwExceptions(std::istream &stream);
    static void consumeEmptyLines(std::istream &stream);
};