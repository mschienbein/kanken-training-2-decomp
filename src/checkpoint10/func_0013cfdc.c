#include "game/render_state.h"

void func_0013cfdc(void)
{
    struct GpuState *state = data_00242fd0;
    if (state->requested_flags & 0x2000)
        func_00133b08();
    if (state->dirty & data_002429f0->command_mode)
        func_001876ac(&state->dirty);
    if (state->dirty & data_002429f0->enabled)
        func_001871e4(&state->dirty);
    if (state->dirty & data_002429f0->texture_state)
        func_00186fc8(&state->dirty);
    if (state->dirty & data_002429f0->feature_mask)
        func_001395c0(&state->dirty);
    state->dirty = 0;
}
