#include "game/sequence.h"
/* The copied thread argument is a writable object pointer: the entries at
 * func_00137984 and func_00149270 access fields through that value. */
void func_00144960(void *const *value, void **out)
{
    if (out)
        *out = *value;
}
