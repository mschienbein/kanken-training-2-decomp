#include "game/display_animation.h"
#include "game/text.h"
void func_00177740(const struct MenuItem *metadata, int *first, int *second)
{
    if (first != 0)
        *first = metadata->text.action_first;
    if (second != 0)
        *second = metadata->text.action_second;
}
