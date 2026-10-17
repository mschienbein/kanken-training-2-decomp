#include "game/input.h"

void func_00123a70(unsigned *held, unsigned *pressed, unsigned *released)
{
    if ((*pressed & 12) == 12 ||
        ((*pressed & 4) && !(*held & 8)) ||
        ((*pressed & 8) && !(*held & 4)))
        *pressed |= 8;
    else
        *pressed &= ~8u;

    if ((*released & 12) == 12 ||
        ((*released & 4) && !(*held & 8)) ||
        ((*released & 8) && !(*held & 4)))
        *released |= 8;
    else
        *released &= ~8u;

    if (*held & 4)
        *held = (*held & ~4u) | 8;
    *pressed &= ~4u;
    *released &= ~4u;
}
