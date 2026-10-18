#include "game/exp_heap.h"

/* [INFERENCE] Original runtime descriptor/class names are unrecovered.
 * The one-word parent layout agrees with the canonical opaque identity. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/runtime_type_info.h */
extern "C" {
extern RuntimeTypeInfo data_002437b0;
}

RuntimeTypeInfo data_002433f8(&data_002437b0);
