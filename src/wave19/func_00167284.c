#include "game/functions.h"

#include "game/w11_control.h"

struct AnimationState *func_00167284(void)
{
    if (!(*(const volatile unsigned char *)&data_0023ecc0 & 1) &&
        func_00191c80(&data_0023ecc0)) {
        struct AnimationState *slider = func_001b43bc(&bss_0024424c);
        /* [INFERENCE] Typed source-only no-op registration; original call
         * identity is erased. It neither stores nor invokes this callback. */
        register_animation_state_cleanup(slider, func_001b44e8, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0023ecc0);
    }
    return &bss_0024424c;
}
