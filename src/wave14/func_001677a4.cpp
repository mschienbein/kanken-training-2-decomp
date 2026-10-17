#include "game/glyph_records.h"
#include "game/runtime.h"
/* [INFERENCE] This constructor-owned object's descriptive name is recovered locally. */

void func_001677a4(W14GlyphList *list, int group, int kind, int count)
{
    int index;
    list->group = group;
    list->count = count;
    list->kind = kind;
    list->objects = (W14GlyphObject **)func_00193110(count * sizeof(W14GlyphObject *), data_002232d5);
    list->states = (W14GlyphState **)func_00193110(list->count * sizeof(W14GlyphState *), data_002232d5);
    list->enabled = (signed char *)func_00193110(list->count, data_002232d5);
    for (index = 0; index < list->count; ++index) {
        W14GlyphObject *object = (W14GlyphObject *)func_0019d788(sizeof(W14GlyphObject));
        if (object != 0)
            object = func_001eb6bc(object);
        list->objects[index] = object;
        list->states[index] = (W14GlyphState *)func_00193110(sizeof(W14GlyphState), data_002232d5);
        W14GlyphState *state = list->states[index];
        state->values[0] = 0;
        state->values[1] = 0;
        state->values[2] = 0;
        state->values[3] = 0;
        list->enabled[index] = 0;
    }
}
