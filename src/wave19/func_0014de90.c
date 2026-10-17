#include "game/aux_effect.h"

int func_0014de90(const struct AudioReadiness *state)
{
    if (!state->ready &&
        (!bss_004b6584.active ||
         (signed char)(bss_004b6584.frame - state->generation) > 2))
        return 0;
    return 1;
}
