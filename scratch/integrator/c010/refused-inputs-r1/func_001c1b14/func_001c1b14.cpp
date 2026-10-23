#include "game/screen.h"
#include "game/object_pool.h"
/* INTEGRATOR: move to include/game/screen.h */
extern "C" {
struct DisplayObject *func_0017a818(const struct NameTable *table, int screen,
                                  int active, float x, float y, float z);
}
struct DisplayHandleGroup {
    DisplayHandle *handles[9];
    void initialize();
};
inline void DisplayHandleGroup::initialize()
{
    for (int i = 0; i < 9; ++i) {
        DisplayHandle *handle = handles[i];
        if (handle != 0 && handle->object == 0) {
            float depth = 1.0f + (float)i * 0.05f;
            const NameTable *table = bss_00290e2c[i];
            DisplayObject *object = func_0017a818(table, 3, 0, 0.0f, 0.0f, depth);
            /* ORIGINAL BUG: label setup is not skipped if allocation fails. */
            if (table->count > 0)
                func_0017a968(object, table, 0, 0);
            handle->object = object;
        }
    }
}
/* INTEGRATOR: differs from scratch/authors/w16d/src/func_001c1b14.c:
 * this typed view names the caller's contiguous nine-handle pointer array;
 * unify the entry prototype and caller view before adoption. */
extern "C" void func_001c1b14(DisplayHandleGroup *group)
{
    group->initialize();
}
