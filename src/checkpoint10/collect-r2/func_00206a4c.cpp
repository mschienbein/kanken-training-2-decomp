#include "game/exp_heap.h"

/* INTEGRATOR: complete RuntimeTypeInfo in include/game/exp_heap.h.
 * [INFERENCE] The one-word base-type link follows its runtime-type accessors. */
struct RuntimeTypeInfo {
    RuntimeTypeInfo *base;
    explicit RuntimeTypeInfo(RuntimeTypeInfo *parent) : base(parent) {}
};

/* INTEGRATOR: move to include/game/exp_heap.h. */
extern "C" {
extern struct RuntimeTypeInfo data_00242f90;
}

RuntimeTypeInfo data_002436d8(&data_00242f90);
