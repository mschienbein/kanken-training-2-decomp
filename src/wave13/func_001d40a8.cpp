#include "game/audio_voice.h"
#include "game/sound_controller.h"
#include "game/resource.h"
#include "game/runtime.h"





static __forceinline void initialize_type(ResourceState001d0164 *state)
{
    state->vtable = data_002247f0;
}

static __forceinline void release_owner(ResourceOwner *owner)
{
    unsigned &bits = owner->resource_bits;
    if ((bits & ~1u) != 0) {
        if (bits & 1u)
            func_0019f568();
        func_0019d5c4(reinterpret_cast<Resource *>(bits & ~1u));
        bits = 0;
    }
}

extern "C" FileSoundController001d40a8 *func_001d40a8(FileSoundController001d40a8 *state)
{
    initialize_type(&state->base);
    if (state->active) {
        func_001d2da4(&state->owner);
        func_001d4850(&state->reset);
        state->active = 0;
    }
    func_001d0164(&state->base);
    state->index = -1;
    release_owner(&state->owner);
    return reinterpret_cast<FileSoundController001d40a8 *>(func_001d017c(&state->base));
}
