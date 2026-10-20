#include "game/transition_controller.h"

/* [INFERENCE] Identity of the removed active-field accessor is unrecoverable. */
extern "C" int transition_active(const struct PairCollection *group)
{
    return (signed char)group->active;
}
