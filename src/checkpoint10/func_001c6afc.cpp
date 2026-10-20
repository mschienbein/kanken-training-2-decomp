#include <preinclude.h>
#include <nw/os/os_Memory.h>

#include "game/render_resource.h"

inline OwnedBufferState::~OwnedBufferState()
{
    bool owned = owns;
    vtable = &data_00223f3c;
    if (owned && buffer)
        allocator->Free(buffer);
}

extern "C" OwnedBufferState *func_001c6afc(OwnedBufferState *object)
{
    object->~OwnedBufferState();
    return object;
}
