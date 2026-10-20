#include "game/functions.h"

#include "game/sound_cache_registration.h"

struct SoundResourceCache *func_001b48c8(void)
{
    if (!(*(const volatile unsigned char *)&data_0023ecec & 1) &&
        func_00191c80(&data_0023ecec)) {
        struct SoundResourceCache *cache = func_001b4f58(&bss_00246ae8);
        /* [INFERENCE] Erased registration identity is unrecovered. This typed
         * source-only helper neither stores nor invokes the real destructor. */
        register_sound_resource_cache_cleanup(cache, func_001b501c, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0023ecec);
    }
    return &bss_00246ae8;
}
