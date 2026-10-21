#include "game/transition_controller.h"

/* [INFERENCE] Identity of the removed active-field accessor is unrecoverable. */
int transition_active(const struct PairCollectionData *group)
{
    return (signed char)group->active;
}
