#include "game/screen.h"

extern void func_001670ec(int operation, const struct NameTable *const *items,
                          int count, int mode, unsigned int flags, int force);

void func_0017a7e4(const struct NameTable *const *items,
                   int count, int mode, unsigned int flags)
{
    func_001670ec(1, items, count, mode, flags, 0);
}
