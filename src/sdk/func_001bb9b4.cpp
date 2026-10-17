#include <stddef.h>
#include "game/waitable_handle.h"


static __forceinline void destroy_completion(WaitableHandle *completion)
{
    func_0019df84(completion);
    if (completion->handle != 0) {
        svcCloseHandle(completion->handle);
        completion->handle = 0;
    }
}

extern "C" LargeHandleOwner *func_001bb9b4(LargeHandleOwner *object)
{
    LargeHandleBlock *block = &object->handles[2];
    destroy_completion(&block->completion);
    --block;
    destroy_completion(&block->completion);
    --block;
    destroy_completion(&block->completion);
    object = (LargeHandleOwner *)((unsigned char *)block - offsetof(LargeHandleOwner, handles));
    func_0019f6b4(object->elements, func_001c3100, sizeof(LargeResourceElement), 1);
    return object;
}
