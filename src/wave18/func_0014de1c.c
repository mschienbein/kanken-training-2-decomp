#include "game/material.h"

struct ResourceRecord {
    unsigned int present;
    unsigned int reserved04[2];
    unsigned int info[3];
    int format;
};

extern struct ResourceArray bss_002d5ea0;

unsigned int *func_0014de1c(int index)
{
    struct ResourceRecord *entry = (struct ResourceRecord *)bss_002d5ea0.buffer + index;
    if (entry->present == 0)
        return 0;
    return entry->info;
}
