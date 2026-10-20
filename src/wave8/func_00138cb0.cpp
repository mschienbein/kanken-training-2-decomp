#include "game/scene_lifecycle.h"

extern "C" nw::snd::internal::driver::Voice *func_00138cb0(nw::snd::internal::driver::Voice *resource)
{
    int i;
    for (i = 0; i < 2; ++i) {
        if (resource->channels[i])
            func_001829d0(resource->channels[i]);
    }
    return resource;
}
