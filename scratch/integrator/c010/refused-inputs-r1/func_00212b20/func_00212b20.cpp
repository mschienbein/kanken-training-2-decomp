#include "game/runtime.h"

/* [INFERENCE] Original class name is unrecovered. The registered destructor
 * func_001d3370 accepts Allocation * and clears its two-word state. */
struct StaticAllocation {
    Allocation state;
    StaticAllocation()
    {
        state.pointer = 0;
        state.count = 0;
    }
    ~StaticAllocation();
};

StaticAllocation data_00242fc0;
