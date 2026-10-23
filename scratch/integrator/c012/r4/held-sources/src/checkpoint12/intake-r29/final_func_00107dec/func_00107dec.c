#include "game/runtime.h"
#include "game/heap_manager.h"

static struct ManagedExpHeap *primary_heap_storage(void)
{
    return &bss_004952f0.primary[1].base;
}

static void *allocate_primary(unsigned size, unsigned alignment, int group, int direction, int mode)
{
    return func_00195104(primary_heap_storage(), size, alignment, group, direction, mode);
}

/* The third input is forwarded by the allocation API but unused here. */
void *func_00107dec(unsigned memory, unsigned format, unsigned options, unsigned size)
{
    unsigned storage_format = format;
    unsigned allocation_size = size;
    unsigned alignment;
    void *allocation;
    if (!bss_004952f0.initialized)
        func_0019f568();
    if (allocation_size == 0)
        return 0;
    alignment = 8;
    allocation = 0;
    switch (storage_format) {
    case 0x100: alignment = 4; break;
    case 0x101: alignment = 0x80; break;
    case 0x102: break;
    case 0x103: alignment = 0x40; break;
    case 0x104:
    case 0x105: alignment = 0x10; break;
    default: func_0019f568(); break;
    }
    switch (memory) {
    case 0x10000:
        allocation = allocate_primary(allocation_size, alignment, 0, 0, 0);
        if (!allocation)
            goto allocation_failed;
        break;
    case 0x20000:
        allocation = func_001177a0(&bss_004952f0.vram, allocation_size, alignment);
        break;
    case 0x30000:
        allocation = func_001177fc(&bss_004952f0.vram, allocation_size, alignment);
        break;
    default:
    allocation_failed:
        allocation = 0;
        func_0019f568();
        break;
    }
    return allocation;
}
