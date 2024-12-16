#include "bitmap.h"
#include "transformations.h"

int main()
{
    Bitmap bitmap(20, 20, Pixel(30, 255, 30));
    std::cout << bitmap.getPixelAt(0, 0);
    bitmap.transformImage(PixelTransformations::negative);
    std::cout << std::endl << bitmap.getPixelAt(0, 0);

    return 0;
}