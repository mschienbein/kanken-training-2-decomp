#include "game/functions.h"
#include "game/hwr_runtime.h"

extern unsigned char bss_004de5cc[];

int func_0014db18(void)
{
    data_00243830 = bss_004de5cc;
    /* [INFERENCE] callee identity: inlined by armlink */
    func_0014c2c8();
    return 0;
}
