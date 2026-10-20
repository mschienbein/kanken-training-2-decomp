#define NW_RELEASE
#include "game/property_descriptor.h"
#include "game/command_state.h"
#include "game/resource_runtime.h"
#include "game/screen.h"
#include "game/prompt.h"



static inline void release_pair(PromptObjects *handle)
{
    if (handle) {
        for (int i = 0; i < 2; ++i) {
            if (handle->objects[i])
                func_00183208(handle->objects[i]);
        }
    }
}

static inline void reset_pair(PromptObjects *handle)
{
    if (handle) {
        handle->objects[0] = 0;
        handle->objects[1] = 0;
    }
}

extern "C" void func_001c1600(PromptManager *owner)
{
    for (int i = 0; i < 1; ++i) {
        if (owner->handles[i]) {
            release_pair(owner->handles[i]);
            reset_pair(owner->handles[i]);
        }
    }
    func_0017a938(data_0023f004, -1, 0);
}
