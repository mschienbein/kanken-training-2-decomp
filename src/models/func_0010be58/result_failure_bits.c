#include "game/result.h"
/* [INFERENCE] Failure-bit accessor inlined by armlink and removed;
 * the historical helper identity is not recovered. */
unsigned result_failure_bits(struct Result result)
{
    return result.value & 0x80000000u;
}
