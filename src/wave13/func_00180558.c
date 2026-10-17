#include "game/service_runtime.h"
/* [INFERENCE] The two indexed words are returned to writable caller buffers. */

void func_00180558(const struct ServiceOutput *state, int index,
                   AuxCallback *first, void **second)
{
    *first = state->callbacks[index];
    *second = state->contexts[index];
}
