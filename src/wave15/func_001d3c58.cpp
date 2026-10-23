#include "game/wave_player.h"

extern "C" void func_001d3c58(W15iAddressCache *cache)
{
    if (cache->active) {
        cache->address = 0;
        cache->length = 0;
        cache->active = 0;
    }
}
