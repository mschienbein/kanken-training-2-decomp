#include "game/waitable_handle.h"
#include "ctr/svc.h"




static __inline WaitableHandle *joined_completion(WaitableHandle *completion)
{
    func_0019df84(completion);
    return completion;
}

extern "C" JoinedHandleOwnerC20 *func_001c2158(JoinedHandleOwnerC20 *object)
{
    WaitableHandle *completion = joined_completion(&object->completion);
    if (completion->handle != 0) {
        svcCloseHandle(completion->handle);
        completion->handle = 0;
    }
    return object;
}
