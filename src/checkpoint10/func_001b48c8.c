#include "game/functions.h"

struct SoundResourceCache;
extern struct SoundResourceCache bss_00246ae8;
extern volatile unsigned data_0023ecec;
extern struct SoundResourceCache *func_001b4f58(struct SoundResourceCache *cache);
extern void func_001b501c(struct SoundResourceCache *cache);

struct SoundResourceCache *func_001b48c8(void)
{
    if (!(*(const volatile unsigned char *)&data_0023ecec & 1) &&
        func_00191c80(&data_0023ecec)) {
        struct SoundResourceCache *cache = func_001b4f58(&bss_00246ae8);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00216140(cache, (ObjectDestructor)func_001b501c, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0023ecec);
    }
    return &bss_00246ae8;
}
