#include <preinclude.h>
#include <nw/os/os_Memory.h>

struct OwnedBufferVTable;
/* [INFERENCE] Partial ABI view; allocator is the canonical native interface. */
struct OwnedBufferState {
    const OwnedBufferVTable *vtable;
    nw::os::IAllocator *allocator;
    bool owns;
    unsigned char reserved09[3];
    void *buffer;
    ~OwnedBufferState();
};
/* INTEGRATOR: move to include/game/buffer_owner.h. */
extern "C" {
extern const OwnedBufferVTable data_00223f3c;
}

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
