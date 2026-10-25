/* Natural public-header identity/ABI corroboration only; no source adoption. */
#include <new>

extern "C" {
extern void *bss_00497574;
void func_0010c1c8(void);

void func_00102cb0(void)
{
    if (bss_00497574 != 0) {
        ::operator delete(bss_00497574);
    }
    func_0010c1c8();
}
}
