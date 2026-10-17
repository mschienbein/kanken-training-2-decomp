#include <preinclude.h>
#include <nw/os/os_Memory.h>

struct CachedBufferVTable;
/* [INFERENCE] Partial ABI view; allocator is the canonical native interface. */
struct CachedBufferState {
    const CachedBufferVTable *vtable;
    nw::os::IAllocator *allocator;
    bool flag08;
    bool owns;
    unsigned char reserved0a[0x12];
    void *buffer;
    ~CachedBufferState();
};
/* INTEGRATOR: move to include/game/buffer_owner.h. */
extern "C" {
extern const CachedBufferVTable data_00223f9c;
}

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
