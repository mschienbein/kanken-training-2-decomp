#include "game/animation.h"
#include "game/screen.h"


struct TimedAnimation {
    float remaining;
    struct AnimationDisplayObject *object;
};

struct TimedAnimationList {
    unsigned reserved00;
    struct TimedAnimation *entries[11];
};



void func_001c2e98(struct TimedAnimationList *list)
{
    struct TimedAnimation *entry;
    int i;
    for (i = 0; i < 11; ++i) {
        entry = list->entries[i];
        if (entry != 0 && entry->remaining > 0.0f) {
            entry->remaining -= func_00191134();
            if (entry->remaining <= 0.0f) {
                if (entry->object != 0)
                    func_0015eaa8(entry->object, 0);
                entry->remaining = 0.0f;
            }
        }
    }
}
