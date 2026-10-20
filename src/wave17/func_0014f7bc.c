#include "game/render_state.h"

/* [INFERENCE] The GL_TEXTURE0-relative index occupies the otherwise reserved
 * word at 0x58 in the shared partial GpuState layout. */


void func_0014f7bc(unsigned texture)
{
    struct GpuState *state = data_00242fd0;
    state->active_texture = texture - 0x84c0;
}
