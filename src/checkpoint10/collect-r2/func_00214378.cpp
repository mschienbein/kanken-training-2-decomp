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
extern RuntimeTypeInfo data_002434cc;
extern RuntimeTypeInfo data_002434bc;
}

/* The compiler-generated initializer initializes this static-storage object. */
RuntimeTypeInfo data_002434bc(&data_002434cc);
