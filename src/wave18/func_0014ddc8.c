#include "game/material.h"


int func_0014ddc8(int index)
{
    struct ResourceRecord *entry = (struct ResourceRecord *)bss_002d5ea0.buffer + index;
    return entry->present ? entry->format : 0;
}
