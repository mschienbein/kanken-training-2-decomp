#include "game/selection_collections.h"

/* The constructor/registration evidence identifies two four-byte cells, not
 * the larger canonical WidgetSlot. The object pointer follows the real
 * ResourceEvent cleanup endpoint; no DisplayObject cast or shadow is used. */
static void release_slot(SelectionCell *slot)
{
    if (slot && slot->object)
        func_00183208(slot->object);
}

static void reset_slot(SelectionCell *slot)
{
    if (slot)
        slot->object = 0;
}

static void clear_owned_cells(SelectionSet2 *state)
{
    for (int i = 0; i < 2; ++i) {
        if (state->cells[i]) {
            SelectionCell *cell = state->cells[i];
            release_slot(state->cells[i]);
            reset_slot(state->cells[i]);
            func_001852f4(cell);
            state->cells[i] = 0;
        }
    }
}

SelectionSet2 *func_001aced0(SelectionSet2 *state)
{
    clear_owned_cells(state);
    return state;
}
