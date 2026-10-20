#include <preinclude.h>
#include <nw/os/os_Memory.h>

#include "game/render_resource.h"

inline CachedBufferState::~CachedBufferState()
{
    bool owned = owns;
    vtable = &data_00223f9c;
    if (owned && buffer)
        allocator->Free(buffer);
}

extern "C" CachedBufferState *func_001c7380(CachedBufferState *object)
{
    object->~CachedBufferState();
    return object;
}
