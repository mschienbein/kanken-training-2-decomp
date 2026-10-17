#include "game/display_animation.h"
void func_00177740(const struct ActionMetadata *metadata, int *first, int *second)
{
    if (first != 0)
        *first = metadata->first;
    if (second != 0)
        *second = metadata->second;
}
