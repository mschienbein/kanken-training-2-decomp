#include "game/scene_lifecycle.h"

void func_0016a27c(struct AudioVoiceGroup *element, int enabled)
{
    if (element->flag17 != enabled) {
        unsigned flags = element->dirty;
        element->flag17 = enabled;
        enabled = flags | 2;
        element->dirty = enabled;
    }
}
