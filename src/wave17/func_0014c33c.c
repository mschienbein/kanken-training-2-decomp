#include "game/functions.h"
extern unsigned data_0024388c;
/* [INFERENCE] The caller supplies zero; this input is unused by the callee. */
extern unsigned func_0014dae0(unsigned unused);

unsigned func_0014c33c(void)
{
    if (data_0024388c != 0)
        return ~0u;
    return func_0014dae0(0);
}
