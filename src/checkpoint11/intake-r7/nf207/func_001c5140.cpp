#include "game/layout_draw.h"
#include "game/graphics_address.h"
extern "C" unsigned int *func_001c5140(unsigned int *output, unsigned int **patch,
                                      const void *source, unsigned int bytes)
{
    *output++ = bytes >> 3;
    *output++ = 0x804f0238u;
    *patch = output;
    *output++ = 0;
    *output++ = func_0018f0ac(source) >> 3;
    unsigned address = func_0018f0ac(output + 2) >> 3;
    *output++ = address;
    *output++ = 1;
    return output;
}
