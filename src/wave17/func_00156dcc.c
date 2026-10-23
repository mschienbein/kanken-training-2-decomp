#include "game/functions.h"
#include "ctr/svc.h"
#include "game/reset_reference.h"

struct KernelHandleOwner {
    CtrHandle handle;
};

extern struct KernelHandleOwner bss_004b35b8[3];


void func_00156dcc(void)
{
    func_0019f6ac(bss_004b35b8, func_001c526c, sizeof(struct KernelHandleOwner), 3);
}
