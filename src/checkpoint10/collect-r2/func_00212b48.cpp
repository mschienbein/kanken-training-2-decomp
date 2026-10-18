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
extern RuntimeTypeInfo data_00242fa0;
extern RuntimeTypeInfo data_0024373c;
}

/* The compiler-generated initializer initializes this static-storage object. */
RuntimeTypeInfo data_0024373c(&data_00242fa0);
