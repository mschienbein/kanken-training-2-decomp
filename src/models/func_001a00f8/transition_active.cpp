#include "game/transition_controller.h"

/* [INFERENCE] armlink-inlined signed transition-state accessor removed by --remove; identity inferred. */
extern "C" signed char r4040_transition_active(const struct PairCollection *group)
{
    return (signed char)group->active;
}
