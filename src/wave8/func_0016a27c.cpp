#include "game/scene_lifecycle.h"

extern "C" void func_0016a27c(nw::snd::internal::driver::Voice *element, int enabled)
{
    if (element->flag17 != enabled) {
        unsigned flags = element->dirty;
        element->flag17 = enabled;
        enabled = flags | 2;
        element->dirty = enabled;
    }
}
