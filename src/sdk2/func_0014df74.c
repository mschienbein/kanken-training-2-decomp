#include "game/input_runtime.h"

/* [INFERENCE] Callers consume the forwarded core-word result. Its status
 * role and signedness are unrecovered; int preserves that evidenced return ABI. */
int func_0014df74(struct InputLatchSnapshot *snapshot, signed char index,
                   const struct ExtendedInputUpdate *update)
{
    snapshot->primary[1][index] = update->value != 0;
    return func_0014dfa8(func_00185044(), index, update);
}
