#include "game/hwr_runtime.h"

int func_00157098(struct HwrCandidate *output, int capacity, unsigned short mode)
{
    int result;
    if (bss_004bfce0.state != 3 || output == 0)
        return -1;
    if (bss_004bfce0.mode_b == 1) {
        if (mode == 0)
            goto normal;
        bss_004bfce0.previous = bss_004bfce0.current;
        bss_004bfce0.current = mode;
    }
    if (mode == 0xff)
        goto special;
normal:
    result = func_00159e94(output, capacity);
    goto finished;
special:
    func_0015b950(*data_00243834.group_count, 0);
    result = func_00159e94(output, capacity);
finished:
    bss_004bfce0.state = 4;
    return result;
}
