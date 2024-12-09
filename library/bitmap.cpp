#include "bitmap.h"
#include "parser.h"

int Bitmap::getWidth() { return width; }
int Bitmap::getHeight() { return height; }
Pixel Bitmap::getPixelAt(int x, int y)
{
    if (!hasPoint(x, y)) throw std::invalid_argument("Provided coordinantes are outside the bitmap");
    if (map == nullptr)
        return Pixel();
    return map[x][y];
}

bool Bitmap::setPixelAt(int x, int y, Pixel newPixel)
{
    if (!hasPoint(x, y) || map == nullptr) return false;
    map[x][y] = newPixel;
    return true;
}

void Bitmap::setDimensionsAndClear(int newWidth, int newHeight, Pixel defaultFill)
{
    if (newWidth > 0 && newHeight > 0)
    {
        width = newWidth;
        height = newHeight;
        clear(defaultFill);
    }
    else
    {
        throw std::invalid_argument("Width and height must be over 0");
    }
}

void Bitmap::freeMemory()
{
    if (map != nullptr)
    {
        for (int x = 0; x < width; x++)
            delete[] map[x];
        delete[] map;
    }
}

bool Bitmap::hasPoint(int x, int y)
{
    return (x >= 0 && x < width && y >= 0 && y < width);
}

void Bitmap::clear(Pixel defaultFill)
{
    freeMemory();

    map = new Pixel *[width];

    for (int x = 0; x < width; x++)
    {
        map[x] = new Pixel[height];

        for (int y = 0; y < height; y++)
        {
            map[x][y] = defaultFill;
        }
    }
}

BITMAP_LOAD_STATUS Bitmap::openStream(std::istream& stream)
{
    return Parser::loadToBitmap(*this, stream);
}

void Bitmap::transformImage(Pixel (*transformFunction)(Pixel))
{
    if (map != nullptr) {
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
    if (map != nullptr)
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
        setDimensionsAndClear(initialWidth, initialHeight, defaultFill);
    }
    else
    {
        throw std::invalid_argument("Width and height must be over 0");
    }
}
