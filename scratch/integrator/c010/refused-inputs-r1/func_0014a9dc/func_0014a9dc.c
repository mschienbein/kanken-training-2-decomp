#include "game/hwr_image.h"

extern int func_0014c4cc(unsigned flags, int mode_a, int mode_b,
                         int width, int height, unsigned limit_a, unsigned limit_b);

static unsigned convert_configuration_flags(unsigned flags)
{
    unsigned converted;
    converted = 0x8000;
    if (flags & 2) converted |= 2;
    if (flags & 4) converted |= 4;
    if (flags & 8) converted |= 8;
    if (flags & 1) converted |= 1;
    if (flags & 0x10) converted |= 0x10;
    if (flags & 0x20) converted |= 0x20;
    if (flags & 0x40) converted |= 0x44;
    if (flags & 0x80) converted |= 0x80;
    if (flags & 0x100) converted |= 0x100;
    if (flags & 0x200) converted |= 0x200;
    if (flags & 0x400) converted |= 0x400;
    if (flags & 0x800) converted |= 0x800;
    if ((flags & 0x1fff) == 0x1fff) converted |= 0x1fff;
    return converted;
}

void func_0014a9dc(unsigned flags)
{
    data_00243818 = flags | 0x8000;
    func_0014c4cc(convert_configuration_flags(flags), 0, 0, 0, 0, 255, 255);
}
