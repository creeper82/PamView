#pragma once
#include "bitmap.h"
#include <cstdint>

class Parser
{
public:
    static void loadToBitmap(Bitmap &bitmap, std::istream &stream, std::function<void(int)> progressHandler = nullptr);
    static void saveBitmapTo(Bitmap &bitmap, std::ostream &stream, FILETYPE filetype, std::function<void(int)> progressHandler = nullptr);

private:
    static std::string readStringSkipComment(std::istream &stream);
    static int readIntSkipComment(std::istream &stream);
    static char readRawChar(std::istream &stream);
    static Pixel readPixel(std::istream &stream, FILETYPE filetype);
    static void throwExceptions(std::istream &stream);
    static void consumeEmptyLines(std::istream &stream);
};