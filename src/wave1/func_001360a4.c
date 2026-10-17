#include "game/runtime.h"
typedef struct BufferOwner {
    unsigned char reserved[0xc08];
    void *buffer;
    unsigned int count;
    unsigned int owned;
} BufferOwner;

extern BufferOwner bss_002d4270;

void func_001360a4(void)
{
    if (bss_002d4270.buffer) {
        if (bss_002d4270.owned)
            func_0019dc20(bss_002d4270.buffer);
        bss_002d4270.buffer = 0;
        bss_002d4270.count = 0;
        bss_002d4270.owned = 0;
    }
}
