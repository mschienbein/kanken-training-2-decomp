#include "game/exp_heap.h"

/* INTEGRATOR: complete RuntimeTypeInfo in include/game/exp_heap.h.
 * [INFERENCE] Original class names are unrecovered. The one-parent layout and
 * constructor agree with nw::ut::internal::RuntimeTypeInfo. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/exp_heap.h. */
extern "C" {
extern RuntimeTypeInfo data_00242e20;
extern RuntimeTypeInfo data_00242e28;
}

/* The compiler-generated initializer initializes this static-storage object. */
RuntimeTypeInfo data_00242e28(&data_00242e20);
