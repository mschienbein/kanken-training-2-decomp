#include "game/selection_components.h"

/* [INFERENCE] Private activation/helper spelling; the canonical flow owner
 * retains its accepted stack-index/current/previous mode fields. */
static void set_selection(W23FlowScreen *state, int index)
{
    state->stack_index = index;
}

static int *current_modes(W23FlowScreen *state)
{
    return state->modes;
}
static int *previous_modes(W23FlowScreen *state)
{
    return state->previous_modes;
}

static void store_mode(W23FlowScreen *state, int previous, int next)
{
    previous_modes(state)[0] = previous;
    current_modes(state)[0] = next;
}

struct MenuActivation {
    int selection, mode;
    MenuActivation() : selection(1), mode(22) {}
};
static void activate(W23FlowScreen *state, MenuActivation activation)
{
    state->modes[1] = activation.mode;
    set_selection(state, activation.selection);
}



void func_001acb38(W23FlowScreen *state)
{
    activate(state, MenuActivation());
    if (state->mode == 0) {
        store_mode(state, current_modes(state)[0], 1);
    } else {
        int previous = current_modes(state)[0];
        int next = state->mode == 1 ? 11 : 16;
        store_mode(state, previous, next);
    }
    /* ORIGINAL BUG: the previous mode saved by store_mode is discarded. */
    previous_modes(state)[0] = -1;
    previous_modes(state)[1] = -1;
}

