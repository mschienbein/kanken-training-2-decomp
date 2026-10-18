#include "game/display_groups.h"

/* [INFERENCE] The loop's purpose is unknown (a delay or leftover loop).
 * The original executes this counted backedge and the compiler retains it.
 * Callers pass a writable collection but ignore the final count. Main ruled
 * this faithful original control flow, not added codegen steering; review QUALIFIED. */
extern "C" int func_001c20e8(W18DisplayCollection *collection)
{
    int count;
    for (count = 0; count < 10; ++count) {
    }
    return count;
}
