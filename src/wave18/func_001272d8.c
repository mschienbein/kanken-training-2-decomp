#include "game/render_state.h"

extern void func_001876ac(unsigned *state);
extern void func_001871e4(unsigned *state);
extern void func_00186fc8(unsigned *state);
extern void func_001395c0(unsigned *state);

void func_001272d8(unsigned *state)
{
    struct GpuState *registers = data_00242fd0;
    if ((*state & data_002429f0->command_mode) && (data_002429f0->command_mode & registers->dirty))
        func_001876ac(state);
    if ((*state & data_002429f0->enabled) && (data_002429f0->enabled & registers->dirty))
        func_001871e4(state);
    if ((*state & data_002429f0->texture_state) && (data_002429f0->texture_state & registers->dirty))
        func_00186fc8(state);
    if ((*state & data_002429f0->feature_mask) && (data_002429f0->feature_mask & registers->dirty))
        func_001395c0(state);
    registers->dirty &= ~*state;
}
