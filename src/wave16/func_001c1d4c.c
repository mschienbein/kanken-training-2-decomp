#include "game/screen.h"

struct W16SingleDisplay {
    struct DisplayHandle *handle;
};

extern const struct NameTable *data_0023efdc;
extern struct DisplayObject *func_0017a818(const struct NameTable *table, int screen,
                                         int active, float x, float y, float z);

void func_001c1d4c(struct W16SingleDisplay *state)
{
    struct DisplayHandle *handle = state->handle;
    if (handle != 0 && handle->object == 0) {
        const struct NameTable *table = data_0023efdc;
        struct DisplayObject *object = func_0017a818(table, 4, 0, 0.0f, 0.0f, 2.2f);
        /* ORIGINAL BUG: label setup is not skipped if allocation fails. */
        if (table->count > 0)
            func_0017a968(object, table, 0, 0);
        handle->object = object;
    }
}
