#pragma once
#include "bitmap.h"

class Parser
{
public:
    static BITMAP_LOAD_STATUS loadToBitmap(Bitmap& bitmap, std::istream& stream);

private:
    static std::string readStringSkipComment(std::istream &stream);
    static int readIntSkipComment(std::istream &stream);
    static Pixel readPixel(std::istream &stream);
};