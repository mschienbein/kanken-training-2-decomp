#include <stddef.h>
#include "game/sound_runtime.h"

struct PlaybackCleanupState {
    const void *vtable;
    unsigned char reserved04[0x40];
    const void *registry_vtable;
    unsigned char reserved48[8];
    const void *secondary_vtable;
};
struct WaveSoundCleanup {
    const void *vtable;
    unsigned char reserved04[0x9b];
    unsigned char state;
    unsigned char reserveda0[0x88];
    PlaybackCleanupState playback;
};

/* INTEGRATOR: move to include/game/sound_runtime.h */
extern "C" {
extern const unsigned data_002246b8[];
PlaybackCleanupState *func_001d9050(PlaybackCleanupState *object);
}

extern "C" WaveSoundCleanup *func_001d3c2c(WaveSoundCleanup *object)
{
    object->vtable = data_002246b8;
    PlaybackCleanupState *playback = func_001d9050(&object->playback);
    /* Recover the complete object from its embedded playback subobject. */
    WaveSoundCleanup *complete = reinterpret_cast<WaveSoundCleanup *>(
        reinterpret_cast<char *>(playback) - offsetof(WaveSoundCleanup, playback));
    complete->vtable = data_0022459c;
    complete->state = 3;
    return complete;
}
