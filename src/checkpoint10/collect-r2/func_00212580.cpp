#include "game/exp_heap.h"

/* [INFERENCE] Original runtime descriptor/class names are unrecovered.
 * The one-word parent layout agrees with the canonical opaque identity. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/runtime_type_info.h */
extern "C" {
extern RuntimeTypeInfo data_00242fa0;
}

RuntimeTypeInfo data_00243764(&data_00242fa0);
