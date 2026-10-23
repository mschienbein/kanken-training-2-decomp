#include "game/menu_controller.h"

/* [INFERENCE] An erased return-only secondary-controller update is evidenced
 * by its receiver setup between the member update and the state update.
 * Its historical identity and receiver type are not recoverable. */
void tu441_update_secondary(struct Tu441SecondaryController *controller)
{
}


/* [INFERENCE] The signed-byte state getter is inferred from the caller's
 * receiver setup, result test and absent retained getter. Identity is unknown. */
signed char tu441_transition_active(const struct TransitionController *controller)
{
    return (signed char)controller->active;
}
