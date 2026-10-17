#include "game/command_group.h"
/* Core and VFP arguments are independently allocated by this ABI. */
extern "C" void func_001d4d48(AudioTransitionState *state, float value,
                              unsigned char mode)
{
    state->value = value;
    state->mode = mode;
}
