#include "game/layout_draw.h"
#include "game/matrix.h"
#include "game/screen.h"


extern "C" void func_001da0e0(LayoutDrawState *state)
{
    state->base.geometry->count = 0;
    LayoutGeometryData *geometry = state->base.geometry;
    geometry->active = 0;
    geometry->word_count = 0;
}
