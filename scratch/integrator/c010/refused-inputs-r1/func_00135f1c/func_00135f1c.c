#include "game/runtime.h"

struct RuntimeRecord44 {
    unsigned active;
    unsigned char reserved04[0x28];
};
extern void *func_0019d798(int heap, int alignment, unsigned size);

void func_00135f1c(int count)
{
    struct RuntimeRecord44 *records = func_0019d798(1, 4, count * sizeof(*records));
    int i;
    bss_0049cbc0.buffer = records;
    bss_0049cbc0.count = count;
    bss_0049cbc0.capacity = 0;
    for (i = 0; i < count; ++i)
        records[i].active = 0;
}
