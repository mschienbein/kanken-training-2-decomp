#include "game/material.h"
int func_0017e948(int index)
{
    struct ResourceDimensions *entry =
        &((struct ResourceDimensions *)bss_002d5ea0.buffer)[index];
    return entry->resource ? entry->height : 0;
}
