#include "game/layout_draw.h"


static unsigned color_channel(unsigned color, unsigned shift)
{
    return (color >> shift) & 255;
}

static unsigned native_color(unsigned color)
{
    return (color_channel(color, 0) << 24) |
           (color_channel(color, 8) << 16) |
           (color_channel(color, 16) << 8) |
           color_channel(color, 24);
}

/* [INFERENCE] Both callers retain the otherwise unused API context input. */
extern "C" void func_0017a518(void *context, LayoutParameterBase **block,
    unsigned red, unsigned green, unsigned blue, unsigned alpha)
{
    LayoutParameterBase *target = *block;
    unsigned color = (red << 24) | (green << 16) | (blue << 8) | alpha;
    target->colors[0].word = native_color(color);
    func_0017a558(target);
}

extern "C" void func_0017a5b0(void *context, LayoutParameterBase **block,
    unsigned red, unsigned green, unsigned blue, unsigned alpha)
{
    LayoutParameterBase *target = *block;
    unsigned color = (red << 24) | (green << 16) | (blue << 8) | alpha;
    target->colors[0].word = native_color(color);
    func_0017a558(target);
}


