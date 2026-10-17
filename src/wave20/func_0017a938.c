#include "game/screen.h"

/* [INFERENCE] The final scalar is forwarded as an uninterpreted 32-bit option.
 * The callees preserve it without arithmetic; the observed cleanup callers pass 0. */
extern void func_001670ec(int mode, struct NameTable *const *descriptors,
                          int count, int selection, int transition,
                          unsigned int option);

void func_0017a938(struct NameTable *const *descriptors, int count,
                   unsigned int option)
{
    func_001670ec(0, descriptors, count, -1, -1, option);
}
