#include "game/scene_lifecycle.h"

void func_0018290c(struct AudioVoiceGroup *resource, int event,
                   void *context)
{
    struct SoundTrack *child = context;
    unsigned notification = 2;
    switch (event) {
    case 0:
        func_00183550(resource);
        break;
    case 1:
        notification = 3;
        func_00183550(resource);
        break;
    case 2:
    case 3:
        notification = 1;
        break;
    }
    if (child->callback)
        child->callback(child, notification, child->callback_data);
    child->task = 0;
    child->enabled = 0;
    child->active = 0;
    child->pending = 0;
    func_00182994(func_00185cc0(), child);
}
