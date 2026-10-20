#include "game/event_route.h"
unsigned func_001f4958(const struct DrawCommandState *state, unsigned count)
{
    unsigned extra = count > 16 ? count * 2 - 32 : 0;
    return ((state->vertex_offset + extra) & 0xfff) >= 0xfe0 ? 32 : 0;
}
