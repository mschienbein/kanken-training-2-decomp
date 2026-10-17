#include "game/functions.h"
#include "game/runtime.h"


extern volatile unsigned int data_00243800;
extern struct PoolList bss_004b9810;

struct PoolList *func_0017e7c4(void)
{
    if ((data_00243800 & 1) == 0 && func_00191c80(&data_00243800)) {
        bss_004b9810.sentinel.next = &bss_004b9810.sentinel;
        bss_004b9810.count = 0;
        bss_004b9810.sentinel.previous = &bss_004b9810.sentinel;
        /* [INFERENCE] callee identity: armlink inlined the return-only call. */
        func_0018f7f0((void *)&data_00243800);
    }
    return &bss_004b9810;
}
