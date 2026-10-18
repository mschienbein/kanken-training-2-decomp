#include "game/exp_heap.h"

/* INTEGRATOR: complete RuntimeTypeInfo in include/game/exp_heap.h.
 * [INFERENCE] The one-word base-type link follows its runtime-type accessors. */
struct RuntimeTypeInfo {
    RuntimeTypeInfo *base;
    explicit RuntimeTypeInfo(RuntimeTypeInfo *parent) : base(parent) {}
};

RuntimeTypeInfo data_00242da4(&data_00242e2c);
