#include "bitmap.h"
#include "parser.h"

int Bitmap::getWidth() { return width; }
int Bitmap::getHeight() { return height; }
bool Bitmap::hasOpenBitmap()
{
    return map != nullptr;
}
Pixel Bitmap::getPixelAt(int x, int y)
{
    if (!hasPoint(x, y)) throw std::invalid_argument("Provided coordinantes are outside the bitmap");
    if (!hasOpenBitmap())
        throw std::invalid_argument("No bitmap is open");
    return map[x][y];
}

bool Bitmap::setPixelAt(int x, int y, Pixel newPixel)
{
    if (!hasPoint(x, y) || !hasOpenBitmap()) return false;
    map[x][y] = newPixel;
    return true;
}

void Bitmap::createBlank(int newWidth, int newHeight, Pixel defaultFill)
{
    if (newWidth > 0 && newHeight > 0)
    {
        freeMemory();
        width = newWidth;
        height = newHeight;
        allocateBitmapMemory(width, height);

        fillBitmap(defaultFill);
    }
    else
    {
        throw std::invalid_argument("Width and height must be over 0");
    }
}

void Bitmap::freeMemory()
{
    if (hasOpenBitmap())
    {
        for (int x = 0; x < width; x++)
            delete[] map[x];
        delete[] map;
        map = nullptr;
        width = 0;
        height = 0;
    }
}

void Bitmap::allocateBitmapMemory(int width, int height)
{
    map = new Pixel *[width];
    for (int x = 0; x < width; x++)
    {
        map[x] = new Pixel[height];
    }
}

bool Bitmap::hasPoint(int x, int y)
{
    return (x >= 0 && x < width && y >= 0 && y < width);
}

void Bitmap::fillBitmap(Pixel defaultFill)
{
    for (int x = 0; x < width; x++)
    {
        map[x] = new Pixel[height];

        for (int y = 0; y < height; y++)
        {
            map[x][y] = defaultFill;
        }
    }
}

void Bitmap::closeBitmap()
{
    freeMemory();
}

BITMAP_LOAD_STATUS Bitmap::openStream(std::istream &stream, void (*progressHandler)(int progressPercent))
{
    return Parser::loadToBitmap(*this, stream, progressHandler);
}

bool Bitmap::saveToStream(std::ostream &stream, FILETYPE filetype, void (*progressHandler)(int))
{
    return Parser::saveBitmapTo(*this, stream, filetype, progressHandler);
}

void Bitmap::transformImage(Pixel (*transformFunction)(Pixel))
{
    if (hasOpenBitmap()) {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                map[x][y] = transformFunction(map[x][y]);
            }
        }
    }
}

void Bitmap::transformImage(Pixel (*transformFunctionWithLevel)(Pixel, int), int level)
{
    if (hasOpenBitmap())
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                map[x][y] = transformFunctionWithLevel(map[x][y], level);
            }
        }
    }
}

Bitmap::Bitmap() : Bitmap(100, 100)
{
}

Bitmap::Bitmap(int initialWidth, int initialHeight, Pixel defaultFill)
{
    map = nullptr;

    if (initialWidth > 0 && initialHeight > 0)
    {
        createBlank(initialWidth, initialHeight, defaultFill);
    }
    else
    {
        throw std::invalid_argument("Width and height must be over 0");
    }
}