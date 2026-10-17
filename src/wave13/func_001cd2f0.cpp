#include "game/resource_cache.h"
#include "game/pool_list.h"



extern "C" void *func_001cd2f0(ResourceProvider10 *provider, const char *name)
{
    void *resource = func_001cc0dc(&provider->cache, name);
    if (!resource) {
        resource = provider->vtable->load(provider, name);
        if (resource)
            func_001cc050(&provider->cache, name, resource, 1);
    }
    return resource;
}
