#include "game/input_runtime.h"

extern void func_001543b8(struct InputSessionContext *state, int channel,
                          unsigned value);

void func_0017e7a0(struct ServiceOutput *output, int channel, unsigned value)
{
    if (output->active)
        func_001543b8(&bss_004b97ac, channel, value);
}
