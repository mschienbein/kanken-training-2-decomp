#include "game/transition_controller.h"

/* [INFERENCE] Identity of the removed active-field accessor is unrecoverable. */
int transition_active(const struct TransitionController *group)
{
    return (signed char)group->active;
}
