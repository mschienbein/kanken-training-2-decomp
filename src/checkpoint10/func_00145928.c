#include "game/resource_tables.h"

extern void func_00181a4c(uintptr_t resource, int first, int last,
                         int mode, unsigned alpha);
extern void func_00186598(void);
extern void func_001811b8(void);
extern unsigned func_0019d8c4(const struct FontFile *file);
extern void *func_0019d798(int heap, int alignment, unsigned size);

void func_00145928(uintptr_t resource, int index)
{
    bss_002d3d60.firstEnabled = 1;
    bss_002d3d60.firstResource = resource;
    bss_002d3d60.firstIndex = index;
    func_00181a4c(resource, index, index, 3, 128);
    func_00186598();
    func_001811b8();
    bss_002d3d60.firstSheets = func_0019d798(2, 4,
        func_0019d8c4((const struct FontFile *)func_001971a0(resource, index)));
}
