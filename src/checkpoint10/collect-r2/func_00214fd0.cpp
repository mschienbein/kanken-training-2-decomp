#include "game/exp_heap.h"

/* [INFERENCE] Runtime ancestry record, matching the canonical opaque type. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/exp_heap.h */
extern "C" {
extern RuntimeTypeInfo data_002434b0;
}

RuntimeTypeInfo data_002434b0(&data_00242e2c);
