#include "game/resource_tables.h"

extern void func_00181a4c(uintptr_t resource, int first, int last,
                         int mode, unsigned alpha);
extern void func_00186598(void);
extern void func_001811b8(void);
extern unsigned func_0019d8c4(const struct FontFile *file);
extern void *func_0019d798(int heap, int alignment, unsigned size);

void func_001458b8(uintptr_t resource, int index)
{
    bss_002d3d60.secondEnabled = 1;
    bss_002d3d60.secondResource = resource;
    bss_002d3d60.secondIndex = index;
    func_00181a4c(resource, index, index, 3, 128);
    func_00186598();
    func_001811b8();
    bss_002d3d60.secondSheets = func_0019d798(2, 4,
        func_0019d8c4((const struct FontFile *)func_001971a0(resource, index)));
}
