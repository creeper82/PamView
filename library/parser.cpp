#include "parser.h"
#include "exceptions.h"
#define COMMENT_CHAR '#'
#define MAX_PIXELS 100000000
#define PROGRESS_BAR_UPDATE_TRESHOLD 10000
// #define STREAM_EOF_EXCEPTION std::invalid_argument("Reached end of stream while reading data (EOF)")
// #define PARSE_NUM_FAILED std::invalid_argument("Failed to parse a number. File is corrupt")
// #define STREAM_CORRUPT_EXCEPTION std::invalid_argument("Stream is corrupt. Fatal error reading the data")

void Parser::loadToBitmap(Bitmap &bitmap, std::istream &stream, std::function<void(int)> progressHandler)
{
    std::string pNumber;
    FILETYPE filetype;
    int maxValue;
    int width;
    int height;
    long pixelCount;
    char *rawInput = nullptr;
    size_t bytesPerPixel = 0;

    pNumber = readStringSkipComment(stream);
    width = readIntSkipComment(stream);
    height = readIntSkipComment(stream);
    maxValue = readIntSkipComment(stream);
    consumeEmptyLines(stream);

    pixelCount = width * height;

    if (width < 1 || height < 1)
        throw bad_dimensions_exception("Width or height was less than 1");
    if (pixelCount > MAX_PIXELS)
        throw too_large_exception("Exceeded maximum supported pixel count");

    if (pNumber == "P1" || pNumber == "P2" || pNumber == "P3" || pNumber == "P4" || pNumber == "P5" || pNumber == "P6")
    {
        filetype = (FILETYPE)(pNumber[1] - '1');

        // only supports maxValue of 255 (8-bit) or 1 for P1
        if (maxValue != 255 && maxValue != 1)
            throw unsupported_maxvalue_exception("This bitmap's color maxvalue is not supported");

        // Read all at once for binary files
        if (filetype > P3) {
          bytesPerPixel = (filetype == P6 ? 3 : 1);
          rawInput = new char[pixelCount * bytesPerPixel];
          stream.read(rawInput, pixelCount * bytesPerPixel);
          throwExceptions(stream);
        }

        bitmap.createBlank(width, height);

        if (progressHandler)
            progressHandler(0);

        int pixelNum = 0;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++, pixelNum++)
            {
                // update progress event every 10000 pixels
                if (progressHandler && pixelNum % PROGRESS_BAR_UPDATE_TRESHOLD == 0)
                    progressHandler(pixelNum / (float)pixelCount * 100);

                if (filetype == P6)
                {
                    bitmap.setPixelAtFast(x, y,
                        Pixel(
                            rawInput[pixelNum * bytesPerPixel],
                            rawInput[pixelNum * bytesPerPixel + 1],
                            rawInput[pixelNum * bytesPerPixel + 2]
                        )
                    );
                }
                else if (filetype == P5)
                {
                    uint8_t value = rawInput[pixelNum * bytesPerPixel];

                    bitmap.setPixelAtFast(x, y, Pixel(value, value, value));
                }
                else if (filetype == P4)
                {
                    uint8_t value = rawInput[pixelNum * bytesPerPixel] == 1 ? 255 : 0;
                    bitmap.setPixelAtFast(x, y, Pixel(value, value, value));
                }
                else
                {
                    bitmap.setPixelAtFast(x, y, readPixel(stream, filetype));
                }
            }
        }

        if (progressHandler)
            progressHandler(100);
        if (rawInput)
            delete[] rawInput;
        rawInput = nullptr;
    }
    else
    {
        throw unsupported_format_exception("This file format is not supported");
    }
}

void Parser::saveBitmapTo(Bitmap &bitmap, std::ostream &stream, FILETYPE filetype, std::function<void(int)> progressHandler)
{
    if (!bitmap.hasOpenBitmap())
        throw no_bitmap_open_exception("No bitmap was open when trying to save");
    if (filetype == P3)
    {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        long pixelCount = width * height;

        if (pixelCount > MAX_PIXELS)
            throw too_large_exception("Bitmap's pixel count too large");

        if (progressHandler)
            progressHandler(0);

        stream
            << "P3" << '\n'
            << "# Created with PamView" << '\n'
            << width << ' ' << height << '\n'
            << 255 << '\n';

        int pixelNum = 0;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++, pixelNum++)
            {
                // update progress event every 10000 pixels
                if (progressHandler && pixelNum % PROGRESS_BAR_UPDATE_TRESHOLD == 0)
                    progressHandler(pixelNum / (float)pixelCount * 100);

                Pixel pixel = bitmap.getPixelAtFast(x, y);
                stream
                    << (int)pixel.r << '\n'
                    << (int)pixel.g << '\n'
                    << (int)pixel.b << '\n';
            }
        }

        if (progressHandler)
            progressHandler(100);
    }
    else
        throw unsupported_format_exception("This format is not supported for saving");
}

std::string Parser::readStringSkipComment(std::istream &stream)
{
    std::string line;

    consumeEmptyLines(stream);

    while (stream.peek() == COMMENT_CHAR)
    {
        getline(stream, line);
    }

    throwExceptions(stream);
    getline(stream, line);

    return line;
}

int Parser::readIntSkipComment(std::istream &stream)
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

char Parser::readRawChar(std::istream &stream)
{
    char value;
    stream.read(&value, 1);
    throwExceptions(stream);
    return value;
}

Pixel Parser::readPixel(std::istream &stream, FILETYPE filetype)
{
    uint8_t r, g, b;

    if (filetype == P6)
    {
        r = readRawChar(stream);
        g = readRawChar(stream);
        b = readRawChar(stream);
    }
    else if (filetype == P3)
    {
        int ir, ig, ib;

        stream >> ir;
        stream >> ig;
        stream >> ib;

        r = ir;
        g = ig;
        b = ib;
    }
    else if (filetype == P5)
    {
        r = g = b = stream.get();
    }
    else if (filetype == P2)
    {
        uint8_t value;
        stream >> value;
        r = g = b = value;
    }
    else if (filetype == P4)
    {
        uint8_t value = (readRawChar(stream) == 1 ? 255 : 0);
        r = g = b = value;
    }
    else if (filetype == P1)
    {
        uint8_t value;
        stream >> value;
        r = g = b = (value == 1 ? 255 : 0);
    }

    return Pixel(r, g, b);
}

void Parser::throwExceptions(std::istream &stream)
{
    if (stream.eof())
        throw stream_corrupt_exception("Unexpectedly reached EOF while reading stream", true);
    if (stream.fail())
        throw stream_corrupt_exception("Failed to parse data from stream", false);
    if (stream.bad())
        throw stream_corrupt_exception("Failure when parsing stream. Could be EOF", true);
}

void Parser::consumeEmptyLines(std::istream &stream)
{
    while (stream.peek() == '\n' || stream.peek() == '\r')
        stream.get();
}
