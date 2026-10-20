#include "game/functions.h"
struct MemorySpan {
    unsigned base;
    unsigned size;
};

extern struct MemorySpan data_002437a0;

int func_0013e6b8(void)
{
    return data_002437a0.size != 0;
}
