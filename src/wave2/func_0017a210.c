#include "game/controller.h"
#include "game/w11_control.h"

int func_0017a210(const struct AnimationState *state)
{
    if (state->finish == state->start)
        return 0;
    if (state->current == state->lower)
        return 2;
    if (state->current == state->upper)
        return 3;
    return 1;
}
