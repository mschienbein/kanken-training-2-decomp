#include "game/command_state.h"

/* [INFERENCE] The final scalar is forwarded as an uninterpreted 32-bit option.
 * The callees preserve it without arithmetic; the observed cleanup callers pass 0. */

void func_0017a938(const struct NameTable *const *descriptors, int count,
                   unsigned int option)
{
    func_001670ec(0, descriptors, count, -1, -1, option);
}
