#include "game/selection.h"

struct W17SelectionOwner {
    void *allocation;
    int selection;
    unsigned int count;
};

extern void func_0019d95c(uintptr_t resource, int index, int force);

void func_001e3548(struct W17SelectionOwner *state)
{
    if (state->selection != -1) {
        struct MetadataEntry entry = data_0023fce4[state->selection];
        func_0019d95c(entry.resource, entry.index, 0);
    }
    state->count = 0;
    state->allocation = 0;
    state->selection = -1;
}
