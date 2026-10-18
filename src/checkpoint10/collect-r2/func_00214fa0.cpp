#include "game/exp_heap.h"

/* [INFERENCE] Runtime ancestry record, matching the canonical opaque type. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/exp_heap.h */
extern "C" {
extern RuntimeTypeInfo data_00243490;
}

RuntimeTypeInfo data_00243490(&data_00243494);
