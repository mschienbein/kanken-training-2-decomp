#include "game/property_descriptor.h"

struct ThreePanelSet {
    struct DisplayHandle *handles[3];
};

/* INTEGRATOR: move to include/game/display_factory.h */
extern "C" {
extern struct DisplayObject *func_0017a818(const struct NameTable *table,
    int layer, int enabled, float x, float y, float z);
}


static struct DisplayObject *create_panel(const struct NameTable *table, float depth)
{
    struct DisplayObject *object = func_0017a818(table, 3, 0, 0.0f, 0.0f, depth);
    if (table->count > 0)
        func_0017a968(object, table, 0, 0);
    return object;
}

extern "C" void func_001c3218(struct ThreePanelSet *set)
{
    int i = 0;
    do {
        struct DisplayHandle *handle = set->handles[i];
        if (handle && !handle->object) {
            float depth = 1.0f + (float)i * 0.05f;
            handle->object = create_panel(bss_00290fc4[i], depth);
        }
    } while (++i < 3);
}
