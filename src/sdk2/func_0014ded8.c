#include "game/input_runtime.h"

extern int func_0014df0c(struct InputController *controller, int index,
                          const struct ShortInputUpdate *update);
/* [INFERENCE] Callers consume the forwarded core-word result. Its status
 * role and signedness are unrecovered; int preserves that evidenced return ABI. */
int func_0014ded8(struct InputLatchSnapshot *snapshot, signed char index,
                   const struct ShortInputUpdate *update)
{
    snapshot->primary[0][index] = update->value != 0;
    return func_0014df0c(func_00185044(), index, update);
}
