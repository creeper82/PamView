#include "parser.h"
#define COMMENT_CHAR '#'
#define MAX_PIXELS 100000000

BITMAP_LOAD_STATUS Parser::loadToBitmap(Bitmap& bitmap, std::istream& stream)
{
    std::string pNumber;
    int maxValue;
    int width;
    int height;
    int pixelCount;

    pNumber = readStringSkipComment(stream);
    width = readIntSkipComment(stream);
    height = readIntSkipComment(stream);
    maxValue = readIntSkipComment(stream);

    pixelCount = width * height;

    if (width < 1 || height < 1) return BAD_DIMENSIONS;
    if (pixelCount > MAX_PIXELS) return TOO_LARGE;

    if (pNumber == "P3") {
        // only supports maxValue of 255
        if (maxValue != 255)
            return UNSUPPORTED_MAXVALUE;
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bitmap.setPixelAt(x, y, readPixel(stream));
            }
        }

        return SUCCESS;
    } else {
        return UNSUPPORTED_FORMAT;
    }
}

std::string Parser::readStringSkipComment(std::istream& stream)
{
    std::string line;

    while (stream.peek() == COMMENT_CHAR && !stream.bad()) {
        getline(stream, line);
    }
    
    if (stream.bad()) return "";
    getline(stream, line);

    return line;
}

int Parser::readIntSkipComment(std::istream& stream)
{
    int num;
    std::string trash;

    while (stream.peek() == COMMENT_CHAR && !stream.bad())
    {
        getline(stream, trash);
    }

    if (stream.bad())
        return -1;
    
    stream >> num;

    return num;
}

Pixel Parser::readPixel(std::istream& stream)
{
    int r, g, b;
    stream >> r;
    stream >> g;
    stream >> b;

    return Pixel(r, g, b);
}
