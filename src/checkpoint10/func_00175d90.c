#include "game/w11_control.h"

void func_00175d90(struct W11jVisualOwner *set)
{
    int i;
    for (i = 0; i < 72; ++i) {
        if (set->formats[i].kind == -1)
            return;
        func_001eaac0(set, i);
    }
}
