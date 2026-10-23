#include "game/material.h"

static void set_render_mode(int mode)
{
    if (mode != 0)
        data_002432b8.mode = 2;
    else
        data_002432b8.mode = 0;
}

unsigned func_0014025c(int mode)
{
    set_render_mode(mode);
    data_00242fc8 = func_001846f8(data_00242fc8, &data_002432b8);
    return sizeof(data_002432b8);
}
