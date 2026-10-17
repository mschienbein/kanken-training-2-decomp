#include "game/aux_effect.h"
#include "game/input_runtime.h"
int func_0014c90c(struct InputLatchSnapshot *flags, int kind, int channel)
{
    flags->current[kind][channel] = 0;
    return 1;
}
