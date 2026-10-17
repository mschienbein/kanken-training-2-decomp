#include "game/property_descriptor.h"
#include "game/command_state.h"
#include "game/resource_runtime.h"
#include "game/screen.h"

struct PairedDisplayHandle {
    DisplayObject *objects[2];
};

struct PairedDisplayOwner {
    unsigned reserved00;
    PairedDisplayHandle *handles[1];
};


static inline void release_pair(PairedDisplayHandle *handle)
{
    if (handle) {
        for (int i = 0; i < 2; ++i) {
            if (handle->objects[i])
                func_00183208(handle->objects[i]);
        }
    }
}

static inline void reset_pair(PairedDisplayHandle *handle)
{
    if (handle) {
        handle->objects[0] = 0;
        handle->objects[1] = 0;
    }
}

extern "C" void func_001c1600(PairedDisplayOwner *owner)
{
    for (int i = 0; i < 1; ++i) {
        if (owner->handles[i]) {
            release_pair(owner->handles[i]);
            reset_pair(owner->handles[i]);
        }
    }
    func_0017a938(data_0023f004, -1, 0);
}
