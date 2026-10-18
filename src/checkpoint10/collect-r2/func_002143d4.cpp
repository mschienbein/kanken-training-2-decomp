#include "game/exp_heap.h"

/* [INFERENCE] NintendoWare-style runtime ancestry record; the canonical
 * declaration is opaque and the initializer establishes its sole pointer. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

RuntimeTypeInfo data_00242e2c(&data_00242e24);
