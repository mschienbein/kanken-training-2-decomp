/* [INFERENCE] reserved04 is an opaque packed command word. The payload
 * exchanges its halfwords; their domain meaning remains unrecovered. */
#include "game/input_runtime.h"
#include "game/aux_effect.h"

/* [INFERENCE] Command constructors are shared; original TU membership unknown. */
static void initialize_command(ShortInputUpdate *command, unsigned flags, const unsigned &value)
{
    if (flags & 1)
        command->value = (unsigned short)value;
    if (flags & 2)
        command->data = (void *)((value >> 16) | (value << 16));
    command->flags = (unsigned short)flags;
}
extern "C" int func_00151a74(AuxEffectState *snapshot, signed char device)
{
    int index = device;
    if (!snapshot->active || snapshot->channel != -1)
        return 0;
    if (index != 0 && index != 1)
        return 0;
    int result = func_0015341c(func_001861e0(), 0, index);
    if (!result)
        return result;
    /* ORIGINAL BUG: unused command fields are indeterminate. */
    ShortInputUpdate command;
    initialize_command(&command, 2, snapshot->reserved04);
    result = func_0014ded8(func_001861e0(), index, &command);
    if (result)
        snapshot->channel = index;
    return result;
}
/* Mask application shares this complete two-function command unit. */
extern "C" int func_00151b04(AuxEffectState *state, unsigned mask)
{
    if (state->channel == 0 || state->channel == 1) {
        unsigned enabled = 0;
        if (state->buffer)
            enabled = state->size;
        if (state->buffer && enabled) {
            /* ORIGINAL BUG: unused command fields are indeterminate. */
            ShortInputUpdate command;
            initialize_command(&command, 1, mask);
            int index = state->channel;
            int result = func_0014ded8(func_001861e0(), (signed char)index, &command);
            if (!(mask & ~(unsigned)result))
                state->enabled = (unsigned char)mask;
            if (!state->enabled)
                state->frame = bss_004b6584.frame;
            return result;
        }
    }
    return 0;
}
