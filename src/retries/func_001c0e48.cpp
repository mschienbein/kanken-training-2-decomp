#include "game/waitable_handle.h"
#include "ctr/svc.h"




static __inline WaitableHandle *joined_completion(WaitableHandle *completion)
{
    func_0019df84(completion);
    return completion;
}

extern "C" JoinedHandleOwner10 *func_001c0e48(JoinedHandleOwner10 *object)
{
    WaitableHandle *completion = joined_completion(&object->completion);
    if (completion->handle != 0) {
        svcCloseHandle(completion->handle);
        completion->handle = 0;
    }
    return object;
}
