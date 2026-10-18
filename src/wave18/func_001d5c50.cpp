#include "game/scene_lifecycle.h"



void func_001d5c50(struct SceneToggle *toggle, bool enabled)
{
    toggle->enabled = enabled;
    struct SoundTrack *child = toggle->child;
    if (child && child->active && (child->enabled != 0) != enabled) {
        child->enabled = enabled;
        func_0016a27c(child->task, enabled);
    }
}
