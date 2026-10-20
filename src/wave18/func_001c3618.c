#include "game/functions.h"
#include "game/resource_runtime.h"
#include "game/command_state.h"

void func_001c3618(struct AssetSlot **slots)
{
    int i;
    for (i = 0; i < 5; ++i) {
        struct AssetSlot *slot = slots[i];
        if (!slot)
            continue;
        if (slot->asset)
            func_00183208(slot->asset);
        slot = slots[i];
        if (slot)
            slot->asset = 0;
    }
    func_0017a938(bss_00290f8c, -1, 0);
}
