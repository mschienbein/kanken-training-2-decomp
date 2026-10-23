#include "game/selection_collections.h"

/* [INFERENCE] Guard release and static-destructor registration were erased by
 * armlink; the native local static retains the actual C++ lifetime semantics. */
PairCollection *func_001b6400()
{
    static PairCollection bss_002470fc;
    return &bss_002470fc;
}
