#include "game/scene_lifecycle.h"
#include "game/audio_voice.h"

void func_0018290c(struct AudioVoiceGroup *resource, int event,
                   void *context)
{
    struct AudioVoice *child = context;
    int notification = 2;
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
    child->group = 0;
    child->flag138 = 0;
    child->flag139 = 0;
    child->flag13a = 0;
    func_00182994(func_00185cc0(), child);
}
