#ifndef GAME_STATIC_ALLOCATION_H
#define GAME_STATIC_ALLOCATION_H
#include "game/runtime.h"
#ifdef __cplusplus
/* The retained initializer owns exactly the canonical Allocation prefix.
 * [INFERENCE] Class identity; the registered cleanup consumes that first
 * member at offset zero and ignores the native destructor result. */
struct StaticAllocation {
    Allocation state;
    StaticAllocation()
    {
        state.pointer = 0;
        state.count = 0;
    }
    ~StaticAllocation();
};

extern "C" { extern StaticAllocation data_00242fc0; }
#endif
#endif
