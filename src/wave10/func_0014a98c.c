#include "game/hwr_image.h"
int func_0014a98c(unsigned width, unsigned height, struct HwrImage *image)
{
    int result;
    struct HwrImageSource source;
    if (!image)
        return -1;
    source.image = image;
    source.option_a = 0;
    source.option_b = 0;
    result = func_0014c360(width, height, 1, 4, source);
    data_00243818.mode = 0x1fff;
    return result;
}
