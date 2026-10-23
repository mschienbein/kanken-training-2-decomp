#include "game/selection_collections.h"

/* [INFERENCE] Guard release and static-destructor registration were erased by
 * armlink; the native local static retains the actual C++ lifetime semantics. */
ScalarCollection *func_001b695c()
{
    static ScalarCollection bss_0024719c;
    return &bss_0024719c;
}
