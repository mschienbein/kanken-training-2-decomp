#include "game/layout_draw.h"
#include "game/matrix.h"
#include "game/screen.h"


extern "C" void func_001da0fc(LayoutParameterBase *state, float width, float height)
{
    float scale_x = width / (float)state->surface->vtable->width(state->surface);
    float scale_y = height / (float)state->surface->vtable->height(state->surface);
    state->scale_x = scale_x;
    state->scale_y = scale_y;
}
