#include "game/pane_descriptors.h"

struct ParameterValue;

extern void func_0017a7e4(const struct NameTable *const *items,
                          int count, int mode, unsigned int flags);

/* [INFERENCE] The caller-supplied parameter collection is unused. */
void func_001b6d2c(struct ParameterValue **parameters)
{
    func_0017a7e4(bss_00290d54, -1, 3, 0x80);
}
