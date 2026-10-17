#include <string.h>

struct HwrOutputState {
    unsigned reserved00;
    unsigned count;
};

extern unsigned bss_004bfbc8[32];
extern struct HwrOutputState data_00243818;

void func_0017d9e0(void)
{
    memset(bss_004bfbc8, 0, sizeof(bss_004bfbc8));
    data_00243818.count = 0;
}
