#include "ctr/svc.h"

struct KernelHandleOwner {
    CtrHandle handle;
};

extern struct KernelHandleOwner bss_004b35b8[3];
extern struct KernelHandleOwner *func_001c526c(struct KernelHandleOwner *object);
extern void func_0019f6ac(struct KernelHandleOwner *array,
                         struct KernelHandleOwner *(*destroy)(struct KernelHandleOwner *),
                         unsigned element_size, unsigned count);

void func_00156dcc(void)
{
    func_0019f6ac(bss_004b35b8, func_001c526c, sizeof(struct KernelHandleOwner), 3);
}
