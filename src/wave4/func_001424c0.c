#include "game/functions.h"

struct MemorySpan {
    unsigned base;
    unsigned size;
};

extern struct MemorySpan data_002437a0;

unsigned func_001424c0(void)
{
    return data_002437a0.size;
}
