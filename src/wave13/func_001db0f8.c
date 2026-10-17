#include "game/command_group.h"
/* Core argument r0 selects the parameter; VFP s0 carries its floating value. */

void func_001db0f8(struct AudioParameterState *state, float value)
{
    state->value = value;
    func_00141560(1, value);
}
