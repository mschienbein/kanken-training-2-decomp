#include "game/resource_cache.h"

extern "C" ResourceCache *func_001c19e0(ResourceCache *cache)
{
    cache->vtable = &data_00223778;
    cache->active = 0;
    cache->count = 0;
    for (unsigned i = 0; i < 4; ++i)
        cache->entries[i] = CacheEntry(0, 0, 0, 0);
    return cache;
}
inline CacheEntry::CacheEntry(unsigned a, unsigned b, unsigned c, unsigned d)
    : first(a), second(b), third(c), fourth(d)
{
}
