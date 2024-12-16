#include "bitmap.h"
#include "parser.h"
#define MAX_PIXELS 100000000
#define PROGRESS_BAR_UPDATE_TRESHOLD 10000

int Bitmap::getWidth() { return width; }
int Bitmap::getHeight() { return height; }
size_t Bitmap::getBitmapMemUsage()
{
    return hasOpenBitmap() ? getMapMemoryUsage(width, height) : 0;
}
size_t Bitmap::getUndoStackMemUsage()
{
    return previousBitmapState.has_value() ? getMapMemoryUsage(previousBitmapState->width, previousBitmapState->height) : 0;
}
size_t Bitmap::getTotalMemUsage()
{
    return getBitmapMemUsage() + getUndoStackMemUsage();
}
bool Bitmap::hasOpenBitmap()
{
    return map != nullptr;
}
Pixel Bitmap::getPixelAt(int x, int y)
{
    if (!hasPoint(x, y))
        throw std::invalid_argument("Provided coordinantes are outside the bitmap");
    if (!hasOpenBitmap())
        throw std::invalid_argument("No bitmap is open");
    return map[x][y];
}

Pixel Bitmap::getPixelAtFast(int x, int y)
{
    return map[x][y];
}

bool Bitmap::setPixelAt(int x, int y, Pixel newPixel, bool skipCommit)
{
    if (!hasPoint(x, y) || !hasOpenBitmap())
        return false;
    if (!skipCommit)
        commitPreChange();
    map[x][y] = newPixel;
    return true;
}

void Bitmap::setPixelAtFast(int x, int y, Pixel newPixel)
{
    map[x][y] = newPixel;
}

void Bitmap::createBlank(int newWidth, int newHeight, Pixel defaultFill)
{
    if (newWidth == width && newHeight == height && hasOpenBitmap())
    {
        fillWithColor(defaultFill, true);
        clearUndoHistory();
        return;
    }
    if (newWidth > 0 && newHeight > 0)
    {
        freeMemory();
        width = newWidth;
        height = newHeight;
        allocateBitmapMemory(width, height);

        fillWithColor(defaultFill, true);
        clearUndoHistory();
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

void Bitmap::freePreviousBitmapStateMemory()
{
    if (previousBitmapState.has_value() && previousBitmapState->map != nullptr)
    {
        int width = previousBitmapState->width;

        for (int x = 0; x < width; x++)
        {
            delete[] previousBitmapState->map[x];
        }

        delete[] previousBitmapState->map;
    }
}

void Bitmap::commitPreChange()
{
    if (!hasOpenBitmap())
        return;
    if (!previousBitmapState.has_value())
        previousBitmapState = SavedBitmapState(nullptr, width, height);

    freePreviousBitmapStateMemory();

    previousBitmapState->map = new Pixel *[width];

    for (int x = 0; x < width; x++)
    {
        previousBitmapState->map[x] = new Pixel[height];
        for (int y = 0; y < height; y++)
        {
            previousBitmapState->map[x][y] = map[x][y];
        }
    }
}

void Bitmap::clearUndoHistory()
{
    freePreviousBitmapStateMemory();
    previousBitmapState.reset();
}

size_t Bitmap::getMapMemoryUsage(int width, int height)
{
    return width * height * sizeof(Pixel);
}

bool Bitmap::hasPoint(int x, int y)
{
    return (x >= 0 && x < width && y >= 0 && y < height);
}

void Bitmap::fillWithColor(Pixel defaultFill, bool skipCommit)
{
    if (!hasOpenBitmap())
        return;
    if (!skipCommit)
        commitPreChange();

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            map[x][y] = defaultFill;
        }
    }
}

void Bitmap::closeBitmap()
{
    freeMemory();
    freePreviousBitmapStateMemory();
}

void Bitmap::openFromStream(std::istream &stream, void (*progressHandler)(int progressPercent))
{
    clearUndoHistory();
    Parser::loadToBitmap(*this, stream, progressHandler);
}

void Bitmap::saveToStream(std::ostream &stream, FILETYPE filetype, void (*progressHandler)(int))
{
    Parser::saveBitmapTo(*this, stream, filetype, progressHandler);
}

void Bitmap::transformImage(Pixel (*transformFunction)(Pixel), void (*progressHandler)(int))
{
    if (hasOpenBitmap())
    {
        commitPreChange();

        if (progressHandler)
            progressHandler(0);

        int pixelNum = 0;
        int pixelCount = width * height;

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++, pixelNum++)
            {
                if (progressHandler && pixelNum % PROGRESS_BAR_UPDATE_TRESHOLD == 0)
                    progressHandler((float)pixelNum / pixelCount * 100);

                map[x][y] = transformFunction(map[x][y]);
            }
        }
        if (progressHandler)
            progressHandler(100);
    }
}

void Bitmap::transformImage(Pixel (*transformFunctionWithLevel)(Pixel, int), int level, void (*progressHandler)(int))
{
    if (hasOpenBitmap())
    {
        commitPreChange();

        if (progressHandler)
            progressHandler(0);

        int pixelNum = 0;
        int pixelCount = width * height;

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++, pixelNum++)
            {
                if (progressHandler && pixelNum % PROGRESS_BAR_UPDATE_TRESHOLD == 0)
                    progressHandler((float)pixelNum / pixelCount * 100);

                map[x][y] = transformFunctionWithLevel(map[x][y], level);
            }
        }

        if (progressHandler)
            progressHandler(100);
    }
}

void Bitmap::undoLastChange()
{
    if (canUndo())
    {
        SavedBitmapState prevState = previousBitmapState.value();
        freeMemory();
        map = prevState.map;
        width = prevState.width;
        height = prevState.height;
        previousBitmapState.reset();
    }
}

bool Bitmap::canUndo()
{
    return previousBitmapState.has_value();
}

Bitmap::Bitmap() {}

Bitmap::Bitmap(int initialWidth, int initialHeight, Pixel defaultFill)
{
    if (initialWidth * initialHeight > MAX_PIXELS || initialWidth > INT32_MAX / initialHeight)
    {
        throw std::invalid_argument("Exceeded max allowed pixel count");
    }
    if (initialWidth > 0 && initialHeight > 0)
    {
        createBlank(initialWidth, initialHeight, defaultFill);
    }
    else
    {
        throw std::invalid_argument("Width and height must be over 0");
    }
}

Bitmap::~Bitmap()
{
    closeBitmap();
}