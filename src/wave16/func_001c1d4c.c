#include "game/screen.h"
#include "game/property_descriptor.h"

struct W16SingleDisplay {
    struct DisplayHandle *handle;
};


void func_001c1d4c(struct W16SingleDisplay *state)
{
    struct DisplayHandle *handle = state->handle;
    if (handle != 0 && handle->object == 0) {
        const struct NameTable *table = data_0023efdc[0];
        struct ResourceEvent *object = func_0017a818(table, 4, 0, 0.0f, 0.0f, 2.2f);
        /* ORIGINAL BUG: label setup is not skipped if allocation fails. */
        if (table->count > 0)
            func_0017a968(object ? &object->base : 0, table, 0, 0);
        handle->object = object;
    }
}
