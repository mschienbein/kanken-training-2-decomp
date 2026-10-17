#include "game/functions.h"
#include "game/runtime.h"
#include "game/screen.h"

StyleOwner *func_001eade0(void)
{
    if ((data_0023ecdc & 1) == 0 && func_00191c80(&data_0023ecdc)) {
        StyleOwner *owner = &bss_00245364;
        for (int i = 0; i < 32; ++i) {
            if (owner->slots[i] == 0)
                owner->slots[i] = new (data_002232d5) W8DisplayHandle;
        }
        /* [INFERENCE] Registration/release call identities were erased by armlink;
         * the concrete callback still receives this owner, and its result is ignored. */
        func_00216140(owner, reinterpret_cast<ObjectDestructor>(func_001eb0d0),
                      func_00100000);
        func_00215ec4(&data_0023ecdc);
    }
    return &bss_00245364;
}
