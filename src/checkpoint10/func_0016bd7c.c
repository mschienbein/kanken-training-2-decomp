#include "game/runtime.h"

struct FileHandleOwner {
    unsigned reserved00;
    unsigned handle;
};

/* [INFERENCE] Role names: the SDK's checked-result helper reports a failing
 * result together with the address of the check (__current_pc). The
 * "no handle" result code and its symbolic name are unknown. */
static __forceinline void CheckResult(unsigned result, unsigned address)
{
    if (result)
        func_0019d6c4(result, address);
}

unsigned func_0016bd7c(struct FileHandleOwner *owner)
{
    CheckResult(owner->handle ? 0 : 0xe0a046da, __current_pc());
    return owner->handle;
}
