#include "game/exp_heap.h"

/* [INFERENCE] The original class name is unrecovered.  The canonical interface
 * already identifies these single-parent descriptors as RuntimeTypeInfo.
 * INTEGRATOR: share this complete definition with the other type initializers. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;
    explicit RuntimeTypeInfo(const RuntimeTypeInfo *base) : parent(base) {}
};

RuntimeTypeInfo data_00243494(&data_00242e2c);
