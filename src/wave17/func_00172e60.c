#include "game/selection.h"

struct W17SelectionResource {
    unsigned int reserved00;
    int selection;
    unsigned int count;
};

extern void func_0019d95c(uintptr_t resource, int index, int force);

void func_00172e60(struct W17SelectionResource *state)
{
    if (state->selection != -1) {
        struct MetadataEntry entry = data_0023fce4[state->selection];
        func_0019d95c(entry.resource, entry.index, 0);
    }
    state->selection = -1;
    state->count = 0;
}
