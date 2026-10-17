#include "game/animation.h"
#include "game/audio.h"
#include "game/pool_list.h"
#include "game/functions.h"
#include <stddef.h>


static inline AnimationIterator begin_registry(PoolList *registry)
{
    return AnimationIterator(registry->sentinel.next);
}

static inline AnimationIterator end_registry(PoolList *registry)
{
    return AnimationIterator(&registry->sentinel);
}


extern "C" void func_00130f54(PoolList *registry, int frames)
{
    AnimationIterator iterator = begin_registry(registry);
    if (iterator != end_registry(registry)) {
        do {
            AnimatedObject &object = *iterator++;
            func_00183f24(&object, frames);
        } while (iterator != end_registry(registry));
    }
}
