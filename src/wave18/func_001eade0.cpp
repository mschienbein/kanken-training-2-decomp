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
        /* [INFERENCE] Typed source-only no-op registration; original call
         * identity is erased. It neither stores nor invokes this callback. */
        register_style_owner_cleanup(owner, func_001eb0d0, func_00100000);
        func_00215ec4(&data_0023ecdc);
    }
    return &bss_00245364;
}
