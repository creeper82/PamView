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
    char* rawInput = nullptr;
    size_t bytesPerPixel = 0;

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
        if (pNumber == "P1" || pNumber == "P2" || pNumber == "P3" || pNumber == "P4" || pNumber == "P5" || pNumber == "P6") {
        filetype = (FILETYPE)(pNumber[1] - '1');

        // only supports maxValue of 255 (8-bit) or 1 for P1
        if (maxValue != 255 && maxValue != 1)
            return UNSUPPORTED_MAXVALUE;

        bitmap.createBlank(width, height);

        if (progressHandler) progressHandler(0);

        // Read all at once for binary files
        if (filetype > P3) {
            bytesPerPixel = (filetype == P6 ? 3 : 1);
            rawInput = new char[pixelCount*bytesPerPixel];
            stream.read(rawInput, pixelCount*bytesPerPixel);
            throwExceptions(stream);
        }

        int pixelNum = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++, pixelNum++) {
                // update progress event every 10000 pixels
                if (progressHandler && pixelNum % PROGRESS_BAR_UPDATE_TRESHOLD == 0)
                    progressHandler(pixelNum / (float)pixelCount * 100);

                if (filetype == P6) {
                    bitmap.setPixelAt(x, y, 
                        Pixel(
                            rawInput[pixelNum*bytesPerPixel],
                            rawInput[pixelNum*bytesPerPixel + 1],
                            rawInput[pixelNum*bytesPerPixel + 2]
                        ), true);
                }
                else if (filetype == P5) {
                    uint8_t value = rawInput[pixelNum*bytesPerPixel];

                    bitmap.setPixelAt(x, y,
                        Pixel(
                            value, value, value
                        ),
                        true
                    );
                }
                else if (filetype == P4) {
                    uint8_t value = rawInput[pixelNum*bytesPerPixel] == 1 ? 255 : 0;
                    bitmap.setPixelAt(x, y, 
                        Pixel(
                            value, value, value
                        ),
                        true
                    );
                }
                else {
                   bitmap.setPixelAt(x, y, readPixel(stream, filetype), true); 
                }
                
            }
        }

        if (progressHandler) progressHandler(100);
        if (rawInput) delete[] rawInput;
        rawInput = nullptr;
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
    catch (std::exception e) {
        if (rawInput) delete[] rawInput;
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

char Parser::readRawChar(std::istream& stream)
{
    char value;
    stream.read(&value, 1);
    throwExceptions(stream);
    return value;
}

Pixel Parser::readPixel(std::istream& stream, FILETYPE filetype)
{
    uint8_t r, g, b;
    if (filetype == P6) {
        r = readRawChar(stream);
        g = readRawChar(stream);
        b = readRawChar(stream);
    }
    if (filetype == P3) {
        r = readIntSkipComment(stream);
        g = readIntSkipComment(stream);
        b = readIntSkipComment(stream);
    }
    if (filetype == P5) {
        uint8_t value = readRawChar(stream);
        r = g = b = value;
    }
    if (filetype == P2) {
        uint8_t value = readIntSkipComment(stream);
        r = g = b = value;
    }
    if (filetype == P4) {
        uint8_t value = (readRawChar(stream) == 1 ? 255 : 0);
        r = g = b = value;
    }
    if (filetype == P1) {
        uint8_t value = (readIntSkipComment(stream) == 1 ? 255 : 0);
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
