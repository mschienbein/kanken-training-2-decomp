#include "game/scene_lifecycle.h"
/* [INFERENCE] Child enumeration reuses an inlined signed index validator. */
static inline bool containsChildIndex(int index)
{
    return index <= 15;
}

void func_001d8960(struct SceneGroup *group, bool enabled)
{
    group->enabled = enabled;
    for (int i = 0; i < 16; ++i) {
        if (!containsChildIndex(i))
            continue;
        SoundTrack *child = group->children[i];
        if (child)
            func_001d4cf4(child, enabled);
    }
}
