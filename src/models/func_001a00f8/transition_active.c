#include "game/transition_controller.h"

/* [INFERENCE] armlink-inlined signed transition-state accessor removed by --remove; identity inferred. */
signed char r4040_transition_active(const struct TransitionController *group)
{
    return (signed char)group->active;
}
