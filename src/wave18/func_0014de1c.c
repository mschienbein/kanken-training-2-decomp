#include "game/material.h"


unsigned int *func_0014de1c(int index)
{
    struct ResourceRecord *entry = (struct ResourceRecord *)bss_002d5ea0.buffer + index;
    if (entry->present == 0)
        return 0;
    return entry->info;
}
