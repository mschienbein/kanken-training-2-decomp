#include "game/sound_track.h"
#include <stdbool.h>

/* [INFERENCE] The word's role is an initialized-state indicator. */

bool func_001d9b9c(const struct InitializedState *state)
{
    return state->initialized != 0;
}
