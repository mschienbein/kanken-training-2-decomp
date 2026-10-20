#include "game/display_groups.h"

/* Callers pass a writable ten-slot collection and ignore the result.
 * [INFERENCE] Capacity-like result semantics; no original loop is claimed. */
extern "C" int func_001c20e8(W18DisplayCollection *collection)
{
    return 10;
}
