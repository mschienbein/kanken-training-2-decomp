#include "game/functions.h"
#include "game/hwr_runtime.h"



int func_00156f04(void)
{
    if (bss_004bfce0.state != 1)
        return -1;
    func_0015cab4();
    /* [INFERENCE] callee identity: inlined by armlink. */
    func_001c7a1c();
    bss_004bfce0.state = 2;
    data_00243834.strokes = 0;
    data_00243834.points = 0;
    data_00243834.encoded_size = 0;
    return 0;
}
