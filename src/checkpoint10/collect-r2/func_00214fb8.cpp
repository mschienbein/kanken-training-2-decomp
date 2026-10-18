#include "game/exp_heap.h"

/* [INFERENCE] Runtime ancestry record, matching the canonical opaque type. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/exp_heap.h */
extern "C" {
extern RuntimeTypeInfo data_00242e1c;
extern RuntimeTypeInfo data_002434a4;
}

RuntimeTypeInfo data_002434a4(&data_00242e1c);
