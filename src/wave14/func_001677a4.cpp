#include <string.h>
#include "game/glyph_records.h"
#include "game/runtime.h"
/* [INFERENCE] This constructor-owned object's descriptive name is recovered locally. */

void func_001677a4(ItemPage *list, int group, int kind, int count)
{
    int index;
    list->group = group;
    list->count = count;
    list->kind = kind;
    list->objects = (MenuItem **)func_00193110(count * sizeof(MenuItem *), data_002232d5);
    list->values = (unsigned short **)func_00193110(list->count * sizeof(unsigned short *), data_002232d5);
    list->flags = (unsigned char *)func_00193110(list->count, data_002232d5);
    for (index = 0; index < list->count; ++index) {
        MenuItem *object = (MenuItem *)func_0019d788(sizeof(MenuItem));
        if (object != 0)
            object = func_001eb6bc(object);
        list->objects[index] = object;
        list->values[index] = (unsigned short *)func_00193110(8 * sizeof(unsigned short), data_002232d5);
        unsigned short *values = list->values[index];
        memset(values, 0, 8 * sizeof(*values));
        list->flags[index] = 0;
    }
}
