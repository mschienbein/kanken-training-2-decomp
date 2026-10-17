#include "game/scene_lifecycle.h"

void func_001d4cf4(struct SoundTrack *parent, bool enabled)
{
    struct SoundTrack *child = parent->children;
    while (child) {
        if (child->active && (child->enabled != 0) != enabled) {
            child->enabled = enabled;
            func_0016a27c(child->task, enabled);
        }
        child = child->next;
    }
}
