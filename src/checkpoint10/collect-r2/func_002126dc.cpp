#include "game/exp_heap.h"

/* [INFERENCE] Original runtime descriptor/class names are unrecovered.
 * The one-word parent layout agrees with the canonical opaque identity. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

RuntimeTypeInfo data_00242fa0(0);
