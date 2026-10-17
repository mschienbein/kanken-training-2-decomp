#include "game/material.h"

struct ResourceRecord {
    unsigned int present;
    unsigned int reserved04[2];
    unsigned int info[3];
    int format;
};

extern struct ResourceArray bss_002d5ea0;

int func_0014ddc8(int index)
{
    struct ResourceRecord *entry = (struct ResourceRecord *)bss_002d5ea0.buffer + index;
    return entry->present ? entry->format : 0;
}
