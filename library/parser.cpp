#include "parser.h"
#define COMMENT_CHAR '#'
#define MAX_PIXELS 100000000
#define PROGRESS_BAR_UPDATE_TRESHOLD 10000
#define STREAM_EOF_EXCEPTION std::invalid_argument("Reached end of stream while reading data (EOF)")
#define PARSE_NUM_FAILED std::invalid_argument("Failed to parse a number. File is corrupt")
#define STREAM_CORRUPT_EXCEPTION std::invalid_argument("Stream is corrupt. Fatal error reading the data")

BITMAP_LOAD_STATUS Parser::loadToBitmap(Bitmap& bitmap, std::istream& stream, void(*progressHandler)(int))
{
    std::string pNumber;
    FILETYPE filetype;
    int maxValue;
    int width;
    int height;
    long pixelCount;

    try {
        pNumber = readStringSkipComment(stream);
        width = readIntSkipComment(stream);
        height = readIntSkipComment(stream);
        maxValue = readIntSkipComment(stream);
    }
    catch (std::invalid_argument e) {
        std::string message = std::string(e.what());

        if (message.find("EOF") != std::string::npos) return STREAM_CORRUPT_EOF;

        return STREAM_CORRUPT;
    }

    pixelCount = width * height;

    if (width < 1 || height < 1) return BAD_DIMENSIONS;
    if (pixelCount > MAX_PIXELS) return TOO_LARGE;

    try {
       if (pNumber == "P1" || pNumber == "P2" || pNumber == "P3") {
        filetype = (FILETYPE)(pNumber[1] - '1');

        // only supports maxValue of 255 (8-bit)
        if (maxValue != 255)
            return UNSUPPORTED_MAXVALUE;

        bitmap.createBlank(width, height);

        if (progressHandler) progressHandler(0);

        int pixelNum = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++, pixelNum++) {
                // update progress event every 10000 pixels
                if (progressHandler && pixelNum % PROGRESS_BAR_UPDATE_TRESHOLD == 0)
                    progressHandler(pixelNum / (float)pixelCount * 100);

                bitmap.setPixelAt(x, y, readPixel(stream, filetype), true);
            }
        }

        if (progressHandler) progressHandler(100);

        return SUCCESS;
    } else {
        return UNSUPPORTED_FORMAT;
    } 
    }
    catch (std::invalid_argument e) {
        std::string message = std::string(e.what());

        if (message.find("EOF") != std::string::npos)
            return STREAM_CORRUPT_EOF;

        return STREAM_CORRUPT;
    }
}

bool Parser::saveBitmapTo(Bitmap &bitmap, std::ostream &stream, FILETYPE filetype, void (*progressHandler)(int))
{
    if (!bitmap.hasOpenBitmap()) return false;
    if (filetype == P3) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        long pixelCount = width * height;

        if (pixelCount > MAX_PIXELS) return false;

        if (progressHandler) progressHandler(0);

        stream
            << "P3" << '\n'
            << "# Created with PamView" << '\n'
            << width << ' ' << height << '\n'
            << 255 << '\n';

        int pixelNum = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++, pixelNum++)
            {
                // update progress event every 10000 pixels
                if (progressHandler && pixelNum % PROGRESS_BAR_UPDATE_TRESHOLD == 0)
                    progressHandler(pixelNum / (float)pixelCount * 100);

                Pixel pixel = bitmap.getPixelAt(x, y);
                stream 
                    << (int)pixel.r << '\n'
                    << (int)pixel.g << '\n'
                    << (int)pixel.b << '\n';
            }
            
        }

        if (progressHandler) progressHandler(100);
    }

    return true;
}

std::string Parser::readStringSkipComment(std::istream &stream)
{
    std::string line;

    consumeEmptyLines(stream);

    while (stream.peek() == COMMENT_CHAR) {
        getline(stream, line);
    }
    
    throwExceptions(stream);
    getline(stream, line);

    return line;
}

int Parser::readIntSkipComment(std::istream& stream)
{
    int num;
    std::string trash;

    consumeEmptyLines(stream);

    while (stream.peek() == COMMENT_CHAR)
    {
        getline(stream, trash);
    }
    
    stream >> num;

    throwExceptions(stream);

    return num;
}

Pixel Parser::readPixel(std::istream& stream, FILETYPE filetype)
{
    int r, g, b;

    if (filetype == P3) {
        r = readIntSkipComment(stream);
        g = readIntSkipComment(stream);
        b = readIntSkipComment(stream);
    }
    if (filetype == P2) {
        int value = readIntSkipComment(stream);
        r = g = b = value;
    }
    if (filetype == P1) {
        int value = (readIntSkipComment(stream) == 1 ? 255 : 0);
        r = g = b = value;
    }
    

    return Pixel(r, g, b);
}

void Parser::throwExceptions(std::istream &stream)
{
    if (stream.eof())
        throw STREAM_EOF_EXCEPTION;
    if (stream.fail())
        throw PARSE_NUM_FAILED;
    if (stream.bad())
        throw STREAM_EOF_EXCEPTION;
}

void Parser::consumeEmptyLines(std::istream &stream)
{
    while (stream.peek() == '\n')
        stream.get();
}
