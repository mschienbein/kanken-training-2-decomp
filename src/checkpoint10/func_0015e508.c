#include "game/transition_state.h"

/* The original reloads the state's address in each arm; the pointer-returning
 * accessors (inlined by the compiler) reproduce that, a hoisted base does not. */
static __inline const float *FirstValue(void)
{
    return &bss_0028d95c[0].previous;
}

static __inline const float *SecondValue(void)
{
    return &bss_0028d95c[1].previous;
}

float func_0015e508(unsigned flags)
{
    if (flags & 3)
        return *FirstValue();
    if (flags & 4)
        return *SecondValue();
    return 0.0f;
}
