#include "game/archive.h"

extern void func_00192720(void *destination, const void *source, unsigned size);

int func_001533b8(const struct ResourceBlock *block, void *output)
{
    const void *source = func_0017ded4(block->reference);
    /* ORIGINAL BUG: unsupported reference types yield null, but the original
     * still copies 0x4c bytes through that null source. */
    func_00192720(output, source, 0x4c);
    return 1;
}
