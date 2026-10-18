#include "game/exp_heap.h"

/* [INFERENCE] Runtime ancestry record, matching the canonical opaque type. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/exp_heap.h */
extern "C" {
extern RuntimeTypeInfo data_00242fa0;
extern RuntimeTypeInfo data_0024372c;
}

RuntimeTypeInfo data_0024372c(&data_00242fa0);
