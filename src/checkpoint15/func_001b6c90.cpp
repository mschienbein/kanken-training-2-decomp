#include "game/pane_descriptors.h"

static void initializeDisplay(struct DisplayHandle *handle, int index, float depth)
{
    struct NameTable *table = bss_00290d54[index];
    struct ResourceEvent *object = func_0017a818(table, 3, 0, 0.0f, 0.0f, depth);
    /* ORIGINAL BUG: label setup is not skipped if allocation fails. */
    if (table->count > 0)
        func_0017a968(&object->base, table, 0, 0);
    handle->object = object;
}

template <int Count>
static void initialize_option_displays(struct DisplayHandle *(&handles)[Count])
{
    for (int i = 0; i < Count; ++i) {
        struct DisplayHandle *handle = handles[i];
        if (handle != 0 && handle->object == 0)
            initializeDisplay(handle, i, 2.0f);
    }
}
extern "C" void func_001b6c90(struct MenuOptionGroup *options)
{
    initialize_option_displays(options->handles);
}
