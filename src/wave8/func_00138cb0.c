#include "game/scene_lifecycle.h"

struct AudioVoiceGroup *func_00138cb0(struct AudioVoiceGroup *resource)
{
    int i;
    for (i = 0; i < 2; ++i) {
        if (resource->channels[i])
            func_001829d0(resource->channels[i]);
    }
    return resource;
}
