#include "game/exp_heap.h"

/* [INFERENCE] One-parent runtime-type metadata; original class name unrecovered. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;

    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/runtime_type_info.h */
extern "C" {
extern RuntimeTypeInfo data_002434cc;
}

RuntimeTypeInfo data_00242e24(&data_002434cc);
