#include "game/input_runtime.h"
#include "game/aux_effect.h"

int func_00151b04(struct AuxEffectState *state, unsigned int mask)
{
    if (state->channel == 0 || state->channel == 1) {
        unsigned int enabled = 0;
        if (state->buffer)
            enabled = state->size;
        if (state->buffer && enabled) {
            struct ShortInputUpdate parameters;
            int result;
            int index;
            struct InputLatchSnapshot *latches;
            /* ORIGINAL BUG: unused command fields remain indeterminate. */
            parameters.value = (unsigned short)mask;
            parameters.flags = 1;
            index = state->channel;
            latches = func_001861e0();
            result = func_0014ded8(latches, (signed char)index, &parameters);
            if (!(mask & ~(unsigned int)result))
                state->enabled = (unsigned char)mask;
            if (!state->enabled)
                state->frame = bss_004b6584.frame;
            return result;
        }
    }
    return 0;
}
