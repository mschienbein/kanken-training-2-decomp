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
extern RuntimeTypeInfo data_00242e1c;
}

/* The compiler-generated initializer initializes the root type's parent. */
RuntimeTypeInfo data_00242e1c(0);
